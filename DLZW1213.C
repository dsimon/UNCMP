/*******************************************************************
* UNCMP - DLZW1213, Version 1.20, created 3-17-90
*
* Dynamic Lempel-Ziv-Welch 12/13 bit uncompression module.
*
* Uncompresses files stored with Crunching (LZW 9-12 bits with RLE
* coding) and Squashing (LZW 9-13).  The basic compression algorithm
* is FC-SWAP (See Storer, James A., _Data Compression Method and
* Theory_, 1989, Computer Science Press).
*
* The great majority of this code came from SQUASH.C by Leslie
* Satensten, which was based on the Unix COMPRESS program.
*
* All of UNCMP is Copyright (C) 1989-90 Derron Simon
*
* This program is free for everyone, but I retain all copyrights over
* it.  Please see USER.MAN for more information.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <dos.h>
#include <signal.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <stdlib.h>
#if defined( __TURBOC__ )
# include <alloc.h>
#elif defined( QC ) || defined ( MSC )
# include <malloc.h>
#else
# include <malloc.h>
#endif
#include "uncmp.h"
#include "archead.h"
#include "global.h"

/* the next two codes should not be changed lightly, as they must not */
/* lie within the contiguous general code space. */

#define FIRST 257              /* first free entry */
#define CLEAR 256              /* table clear output code */

/* The tab_suffix table needs 2**BITS characters.  We                     */
/* get this from the beginning of htab.  The output stack uses the rest   */
/* of htab, and contains characters.  There is plenty of room for any     */
/* possible stack (stack used to be 8000 characters).                     */

#define MAXCODE(n_bits)      (( 1<<(n_bits)) - 1)

#define tab_prefixof(i)      codetab[i]
#define tab_suffixof(i)      ((unsigned char FAR *)(htab))[i]
#define de_stack             ((unsigned char FAR *)&tab_suffixof(1<<BITS))

#define BITS       13          /* could be restricted to 12 */
#define INIT_BITS  9           /* initial number of bits/code */

#define HSIZE      9001                /* 91% occupancy */

long FAR *htab;
unsigned short FAR *codetab;
short hsize;                   /* for dynamic table sizing */



/* all of the following are NEAR to speed things up! */

int NEAR clear_flg = 0;
int NEAR n_bits;               /* number of bits/code */
int NEAR max_bits = BITS;      /* user settable max # bits/code */
int NEAR maxcode;              /* maximum code, given n_bits */
int NEAR maxmaxcode = 1 << BITS;/* should NEVER generate this code */
int NEAR free_ent = 0;         /* first unused entry */

void dlzw_decomp(FILE * in, FILE * out, int squash)
{
    unsigned char FAR *stackp;
    int finchar;
    int oldcode;
    int code;
    int incode;

    if ((htab = (long FAR *) fmalloc(sizeof(long) * HSIZE)) == NULL)
       mem_error();
    if ((codetab = (unsigned short FAR *) fmalloc(sizeof(unsigned short) * HSIZE)) == NULL)
       mem_error();

    if (squash) {
         max_bits = BITS;
         hsize = 9001;
         }
    else {                /* UnCrunch */
       hsize = 5003;

       /* every Crunched file must start with a byte equal to 12, */
       /* the maximum bit size of the pointer-length pair */

       if (!sizeleft) {        /* no bytes in file */
           ffree(htab);
           ffree(codetab);
           return;
           }

         sizeleft--;
         if (12 != (max_bits = getc(in))) {
              read_error();
         }
    }

    maxmaxcode = 1 << max_bits;

    /* start of decompression */

    maxcode = MAXCODE(INIT_BITS);
    n_bits = INIT_BITS;

    for (code = 255; code >= 0; code--) {
       tab_suffixof(code) = code;
    }
    free_ent = FIRST;
    incode = finchar = oldcode = getcode(in);
    if (oldcode == EOF) {              /* EOF already? */
       ffree(codetab);
       ffree(htab);
       return;                         /* Get out of here */
       }

    if (squash) {
         add1crc((char)incode);
         putc((char)incode, out);
         }
    else putc_rle((char) incode, out);

    stackp = de_stack;

    while ((code = getcode(in)) > -1) {
       if (code == CLEAR) {
           for (code = 255; code >= 0; code--)
               tab_prefixof(code) = 0;
           clear_flg = 1;
           free_ent = FIRST - 1;
           if ((code = getcode(in)) == -1) {   /* O, untimely death! */
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

       /* the following code for squashing used to use memrev() to */
       /* reverse the order and then output using fread.  The following */
       /* method was tested to be faster when using setvbuf() */

       /* characters are read in reverse order from the stack (like any */
       /* stack) and then output. */

       if (squash) {
           do {
               add1crc(*--stackp);
               putc(*stackp, out);
           } while (stackp > de_stack);
       } else {               /* crunch */
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

    /* it's important to free all memory used, so UNCMP will run on systems */
    /* with limited RAM */

    ffree(htab);
    ffree(codetab);
}

