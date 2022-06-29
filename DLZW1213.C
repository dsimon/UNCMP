/*******************************************************************
* UNCMP - DLZW1213, Version 1.03, created 6-28-89
*
* Dynamic Lempel-Ziv-Welch 12/13 bit uncompression module.
*
* Uncompresses files stored with Crunching (LZW 9-12 bits with RLE
* coding) and Squashing (LZW 9-13).  The basic compression algorithm
* is FC-SWAP (See Storer, James A., _Data Compression Method and
* Theory_, 1989, Computer Science Press).
*
* The great majority of this code came from SQUASH.C by Leslie
* Satensten, which was based on the Unix COMPRESS program which is
* in the Public Domain.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "dlzw1213.h"
#include "uncmp.h"
#include "archead.h"
#include "global.h"

long headerlength;

void decomp(FILE *,FILE *);  /* function only used by dlzw_decomp() */
int PASCAL getcode(FILE *);

unsigned char rmask[9] = {   /* for use with getcode() */
    0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
    };

int PASCAL getcode(FILE *in)
{
    static char iobuf[BITS];
    register int code;
    static int offset = 0, size = 0;
    register int r_off, bits;
    register unsigned char *bp = iobuf;

    if (clear_flg > 0 || offset >= size || free_ent > maxcode) {

         /* If the next entry will be too big for the current code */
         /* size, then we must increase the size.  This implies */
         /* reading a new buffer full, too. */

         if (free_ent > maxcode) {              n_bits++;
              if (n_bits == max_bits)
                   maxcode = maxmaxcode;    /* won't get any bigger now */
              else
                   maxcode = MAXCODE(n_bits);
         }
         if (clear_flg > 0) {
              maxcode = MAXCODE(INIT_BITS);
              n_bits = INIT_BITS;
              clear_flg = 0;
         }
         for (size = 0; size < n_bits; size++) {
              if ((code = getc_pak(in)) == EOF)
                   break;
              else
                   iobuf[size] = code;
         }

         if (size <= 0)
              return -1;     /* end of file */

         offset = 0;

         /* Round size down to integral number of codes */

         size = (size << 3) - (n_bits - 1);
    }
    r_off = offset;
    bits = n_bits;

    /* Get to the first byte. */

    bp += (r_off >> 3);
    r_off &= 7;

    /* Get first part (low order bits) */

    code = (*bp++ >> r_off);
    bits -= (8 - r_off);
    r_off = 8 - r_off;  /* now, offset into code word */

    /* Get any 8 bit parts in the middle (<=1 for up to 16 bits). */

    if (bits >= 8) {
         code |= *bp++ << r_off;
         r_off += 8;
         bits -= 8;
    }

    /* high order bits. */

    code |= (*bp & rmask[bits]) << r_off;
    offset += n_bits;

    return code;
}

int dlzw_decomp(FILE *in, FILE *out, int arctype, long size)
{
    headertype = arctype;
    headerlength = size;

    if (headertype == 8) {           /* UnCrunch */
         hsize = 5003;

         /* every Crunched file must start with a byte equal to 12, */
         /* the maximum bit size of the pointer-length pair */

         if (12 != (max_bits = getc_pak(in))) {
              read_error();
         }
    } else {                      /* UnSquash */
         max_bits = BITS;
         hsize = 9001;
    }
    maxmaxcode = 1 << max_bits;
    decomp(out,in);
    return(0);
}

void decomp(FILE *out, FILE *in)
{
    register unsigned char *stackp;
    register int finchar;
    register int code, oldcode;
    int incode;

    /* As above, initialize the first 256 entries in the table */

    maxcode = MAXCODE(INIT_BITS);
    n_bits = INIT_BITS;

    /* why does the code run more slowly when the "(unsigned int) 0" is */
    /* removed? */

    for (code = 255; code >= 0; code--) {
         tab_suffixof(code) = (unsigned char) code | (unsigned int) 0;
    }
    free_ent = FIRST;
    incode = finchar = oldcode = getcode(in);
    if (oldcode == EOF)  /* EOF already? */
         return;         /* Get out of here */
    add1crc(incode);     /* calc the crc */

    if (headertype==8)
         lastc = (char) incode;

    fwrite((char *) &incode, sizeof(char), 1, out);   /* first code must be 8 */
                                                      /* bits = char */
    stackp = de_stack;

    while ((code = getcode(in)) > -1) {
         if (code == CLEAR) {
              for (code = 255; code >= 0; code--)
                   tab_prefixof(code) = 0;
              clear_flg = 1;
              free_ent = FIRST - 1;
              if ((code = getcode(in)) == -1) { /* O, untimely death! */
                   break;
              }
         }
         incode = code;

         /* Special case for KwKwK string */

         if (code >= free_ent) {
              *stackp++ = finchar;
              code = oldcode;
         }

         /* Generate output characters in reverse order Stop if input */
         /* code is in range 0..255 */

         while (code >= 256) {
              *stackp++ = tab_suffixof(code);
              code = tab_prefixof(code);
         }
         *stackp++ = finchar = tab_suffixof(code);

         /* the following code for headertype 9 used to use memrev() to */
         /* reverse the order and then output using fread.  The following */
         /* method was tested to be faster */

         /* characters are read in reverse order from the stack (like any */
         /* stack) and then output. */

         if (headertype == 9) {
              do
                   putc_pak(*--stackp, out);
              while (stackp > de_stack);
         } else {  /* headertype==8  */
              do
                   putc_rle(*--stackp, out);
              while (stackp > de_stack);
         }

         /* Generate the new entry */

         if ((code = free_ent) < maxmaxcode) {
              tab_prefixof(code) = (unsigned short) oldcode;
              tab_suffixof(code) = finchar;
              free_ent = code + 1;
         }

         /* Remember previous code */

         oldcode = incode;
    }
}

