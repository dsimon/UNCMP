/*******************************************************************
* UNCMP - PACK, Version 1.03, created 6-28-89
*
* Uncompresses files saved using the pack method (3).  Pack uses
* RLE compression.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "archead.h"
#include "global.h"
#include "uncmp.h"

#define MAXOUTSIZE 16384

void rle_decomp(FILE *in, FILE *out)
{
    register int c;
    char *buffer=NULL;

    if ((buffer = (char *)malloc(sizeof(char)*MAXOUTSIZE)) == NULL) {

         /* uncompress char by char if no room for buffer */

         while ((c=getc_pak(in)) != EOF)
              putc_rle(c,out);
         }

    while (sizeleft >= MAXOUTSIZE) {
         if (fread(buffer,sizeof(char),MAXOUTSIZE,in) != MAXOUTSIZE)
              read_error();
         for(c=0;c != MAXOUTSIZE;c++)
              putc_rle(buffer[c],out);
         sizeleft -= MAXOUTSIZE;
         }

    if (fread(buffer,sizeof(char),sizeleft,in) != sizeleft)
         read_error();
    for(c=0;c != sizeleft;c++)
         putc_rle(buffer[c],out);
    sizeleft = 0;
    free(buffer);
    }

