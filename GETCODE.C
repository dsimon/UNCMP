/*******************************************************************
* UNCMP - GETCODE, Version 1.04, created 7-03-89
*
* Bit input for DLZW1213.
*
* Inputs the correct number of bits for decompress() in module
* DLZW1213 and does basic housekeeping.  This function has been
* isolated so it can be re-written.
*
* The great majority of this code came from SQUASH.C by Leslie
* Satensten, which was based on the Unix COMPRESS program which is
* in the Public Domain.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "uncmp.h"
#include "archead.h"
#include "global.h"

#define MAXCODE(n_bits)      (( 1<<(n_bits)) - 1)

#define BITS       13        /* maximum used by Squashing */
#define INIT_BITS  9         /* initial number of bits/code */

extern int NEAR free_ent;
extern int NEAR max_bits;
extern int NEAR maxcode;
extern int NEAR maxmaxcode;
extern int NEAR clear_flg;
extern int NEAR n_bits;

int getcode(FILE *in)
{
    static char iobuf[BITS];
    register int code;
    static int offset = 0;
    static int size = 0;
    register int r_off;
    int bits;
    unsigned char *bp = iobuf;

    static unsigned char rmask[9] = {   /* for use with getcode() */
         0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
         };

    if (clear_flg > 0 || offset >= size || free_ent > maxcode) {

         /* set size to a register variable.  Since size is used often, and */
         /* only two registers may be defined in MSC, I'm using r_off instead */
         /* of size, because r_off is a register variable. */

         r_off = size;

         /* If the next entry will be too big for the current code */
         /* size, then we must increase the size.  This implies */
         /* reading a new buffer full, too. */

         if (free_ent > maxcode) {
              n_bits++;
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
         for (r_off = 0; r_off < n_bits; r_off++) {
              if (!sizeleft) break;  /* if EOF */
              sizeleft--;
              code = getc(in);
              iobuf[r_off] = code;
         }

         if (r_off <= 0)
              return -1;     /* end of file */

         offset = 0;

         /* Round size down to integral number of codes */

         r_off = (r_off << 3) - (n_bits - 1);

         size = r_off;       /* set size back to r_off */
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
