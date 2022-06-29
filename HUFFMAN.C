/*******************************************************************
* UNCMP - HUFFMAN, Version 1.20, created 3-17-90
*
* Classic Huffman with RLE uncompression module.
*
* The great majority of this code came from SQUPRT33 by Theo Pozzy
* which borrowed code from SQ and USQ, and USQ by Richard Greenlaw.
* Both of the above mentioned programs are in the Public Domain.
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

#include <stdlib.h>
#include <stdio.h>
#if defined( __TURBOC__ )
# include <alloc.h>
#else
# include <malloc.h>
#endif
#include "uncmp.h"
#include "archead.h"
#include "global.h"

#define SQEOF      256         /* Squeeze EOF */
#define NUMVALS    257         /* 256 data values plus SQEOF */

struct sq_tree {               /* decoding tree */
    int children[2];           /* left, right */
};                             /* use large buffer */

void sq_decomp(FILE * in, FILE * out)
{                              /* initialize Huffman unsqueezing */
    register int i;            /* generic loop index */
    register int bitpos;       /* last bit position read */
    int curbyte;               /* last byte value read */
    int numnodes;
    struct sq_tree FAR *dnode;

    /* alloc memory for decoding tree */

    if ((dnode = (struct sq_tree FAR *) fmalloc(sizeof(struct sq_tree) * NUMVALS)) == NULL)
       mem_error();

    /* get number of nodes in tree, this uses two character input calls */
    /* instead of one integer input call for speed */

    if (!(sizeleft - 1) <= 0)
       numnodes = EOF;
    sizeleft -= 2;
    numnodes = getc(in) | (getc(in) << 8);

    if ((numnodes < 0) || (numnodes >= NUMVALS)) {
       printf("File has invalid decode tree\n");
       exit(1);
    }
    /* initialize for possible empty tree (SQEOF only) */

    dnode[0].children[0] = -(SQEOF + 1);
    dnode[0].children[1] = -(SQEOF + 1);

    for (i = 0; i < numnodes; ++i) {   /* get decoding tree from file */
       if ((sizeleft - 3) <= 0) {
           printf("File has invalid decode tree\n");
           exit(1);
       }
       sizeleft -= 4;
       dnode[i].children[0] = getc(in) | (getc(in) << 8);
       dnode[i].children[1] = getc(in) | (getc(in) << 8);
    }

    bitpos = 8;                        /* set to above read condition */

    while (i != EOF) {
       for (i = 0; i >= 0;) {  /* traverse tree               */
           if (++bitpos > 7) {
               if (!sizeleft) {
                   ffree(dnode);
            return;
            }
               sizeleft--;
               curbyte = getc(in);
               bitpos = 0;

               /* move a level deeper in tree */

               i = dnode[i].children[1 & curbyte];
           } else
               i = dnode[i].children[1 & (curbyte >>= 1)];
       }

       /* decode fake node index to original data value */

       i = -(i + 1);

       /* decode special endfile token to normal EOF */

       i = (i == SQEOF) ? EOF : i;
       if (i != EOF)
       putc_rle(i, out);
    }

    /* free up decoding table for later use */

    ffree(dnode);

}
