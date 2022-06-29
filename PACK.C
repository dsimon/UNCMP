/*******************************************************************
* UNCMP - PACK, Version 1.10, created 3-17-90
*
* Uncompresses files saved using the pack method (3).  Pack uses
* RLE compression.
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
#include <stdlib.h>
#if defined( __TURBOC__ )
# include <alloc.h>
#else
# include <malloc.h>
#endif
#include "uncmp.h"
#include "archead.h"
#include "global.h"

#define MAXOUTSIZE 16384

void rle_decomp(FILE * in, FILE * out)
{
    register int c;
    char *buffer = NULL;

    if ((buffer = (char *) malloc(sizeof(char) * MAXOUTSIZE)) == NULL) {

       /* uncompress char by char if no room for buffer */

       while (sizeleft) {
           sizeleft--;
           c = getc(in);
           putc_rle(c, out);
       }
    }
    while (sizeleft >= MAXOUTSIZE) {
       if (fread(buffer, sizeof(char), MAXOUTSIZE, in) != MAXOUTSIZE)
           read_error();
       for (c = 0; c != MAXOUTSIZE; c++)
           putc_rle(buffer[c], out);
       sizeleft -= MAXOUTSIZE;
    }

    if (fread(buffer, sizeof(char), sizeleft, in) != sizeleft)
       read_error();
    for (c = 0; c != sizeleft; c++)
       putc_rle(buffer[c], out);
    sizeleft = 0;
    free(buffer);
}
