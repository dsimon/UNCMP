/*******************************************************************
* UNCMP - RLE, Version 1.20, created 3-17-90
*
* putc_rle() function used by SLZW.C GETCODE.C and PACK.C
*
* Does all the RLE decoding for each compression type which
* requires RLE.
*
* The great majority of this code came from SQUASH.C by Leslie
* Satensten.
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
#include <limits.h>
#include "archead.h"
#include "uncmp.h"
#include "global.h"

/* run-length-encoding marker byte */

#define DLE 0x90               /* repeat sequence marker */

/* run-length-encoding packing states */

#define NOHIST     0           /* don't consider previous input */
#define INREP      1           /* sending a repeated value */

/* putc_rle outputs bytes to a file unchanged, except that runs */
/* more than two characters are compressed to the format:       */
/*       <char> DLE <number>                                    */
/* When DLE is encountered, the next byte is read and putc_rle  */
/* repeats the previous byte <number> times.  A <number> of 0   */
/* indicates a true DLE character.                              */

void putc_rle(unsigned char c, FILE * out)
{                              /* put RLE coded bytes */
    static char lastc;

    switch (state) {           /* action depends on our state */

       case NOHIST:            /* no previous history */
           if (c == DLE)       /* if starting a series */
               state = INREP;  /* then remember it next time */
           else {
               add1crc(c);
               putc(lastc = c, out);   /* else nothing unusual */
           }
           return;

       case INREP:             /* in a repeat */
           if (c)              /* if count is nonzero */
               while (--c) {   /* then repeatedly ... */
                   add1crc(lastc);
                   putc(lastc, out);   /* ... output the byte */
               }
           else {
               add1crc(DLE);
               putc(DLE, out); /* else output DLE as data */
           }
           state = NOHIST;     /* back to no history */
           return;
    }
}
