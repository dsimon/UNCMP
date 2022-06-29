/*******************************************************************
* UNCMP - UNSTORE, Version 1.20, created 3-17-90
*
* Uncompress archives stored with method 1 and 2 (storing).
*
* This file will copy files using a large buffer.  The buffer is
* MAXOUTSIZE large.
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
#include <alloc.h>
#else                          /* MSC */
#include <malloc.h>
#endif
#include "uncmp.h"
#include "archead.h"
#include "global.h"

#define MAXOUTSIZE 16384

void store_decomp(FILE * in, FILE * out)
{
    int c;
    char *buffer = NULL;

    /* first time initialization */

    if ((buffer = (char *) malloc(sizeof(char) * MAXOUTSIZE)) == NULL) {

       /* do char by char if no room for buffer */

       while (!sizeleft) {
           sizeleft--;
           c = getc(in);
           add1crc(c);
           putc(c, out);
       }
       return;
    }
    while (sizeleft >= MAXOUTSIZE) {
       if (fread(buffer, sizeof(char), MAXOUTSIZE, in) != MAXOUTSIZE)
           read_error();
       addcrc(buffer, MAXOUTSIZE);
       fwrite(buffer, sizeof(char), MAXOUTSIZE, out);
       sizeleft -= MAXOUTSIZE;
    }

    if (fread(buffer, sizeof(char), sizeleft, in) != sizeleft)
       read_error();
    addcrc(buffer, sizeleft);
    fwrite(buffer, sizeof(char), sizeleft, out);

    /* free the buffer before exiting */

    free(buffer);
    sizeleft = 0;
}
