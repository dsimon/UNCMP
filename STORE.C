/*******************************************************************
* UNCMP - UNSTORE, Version 1.03, created 6-28-89
*
* Uncompress archives stored with method 1 and 2 (storing).
*
* This file will copy files using a large buffer.  The buffer is
* MAXOUTSIZE large.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifdef __TURBOC__
#include <alloc.h>
#else /* MSC */
#include <malloc.h>
#endif
#include "archead.h"
#include "global.h"
#include "uncmp.h"

#define MAXOUTSIZE 16384

void store_decomp(FILE *in, FILE *out)
{
    int c;
    char *buffer=NULL;

    /* first time initialization */

    if ((buffer = (char *)malloc(sizeof(char)*MAXOUTSIZE)) == NULL) {

         /* do char by char if no room for buffer */

         while ((c=getc_pak(in)) != EOF)
              putc_pak(c,out);
         return;
         }

    while (sizeleft >= MAXOUTSIZE) {
         if (fread(buffer,sizeof(char),MAXOUTSIZE,in) != MAXOUTSIZE)
              read_error();
         addcrc((char *)buffer,MAXOUTSIZE);
         fwrite(buffer,sizeof(char),MAXOUTSIZE,out);
         sizeleft -= MAXOUTSIZE;
         }

    if (fread(buffer,sizeof(char),sizeleft,in) != sizeleft)
         read_error();
    addcrc((char *)buffer,sizeleft);
    fwrite(buffer,sizeof(char),sizeleft,out);

    /* free the buffer before exiting */

    free(buffer);
    sizeleft = 0;
    }


