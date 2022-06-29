/*******************************************************************
* UNCMP - CRC, Version 1.20, created 3-17-90
*
* Console output routine.
*
* Uncompresses to stdout.  No output error checking, because stdout
* always seems to report errors with ferror().
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
#include "uncmp.h"
#include "archead.h"
#include "global.h"

void con_output(FILE *in, char *filename)
{
    crc = 0;
    sizeleft = archead.size;
    state = 0;                 /* set to NOHIST for rle packing */

    printf("Extracting to console %s:\n\n",filename);

    if (archead.atype == 10 || archead.atype == 11) {
         if (warning)
              printf("Cannot extract Crushed or Distilled files!\n\n");
         errors++;
         fseek(in, archead.size, 1);
         return;
         }
    if (archead.atype > 11) {
         if (warning)
              printf("File uses unknown compression type, I think you need a newer version of UNCMP\n");
         fseek(in, archead.size, 1);
         errors++;
         return;
         }

    douncmp(in, stdout);       /* uncompress to stdout */

    printf("\n\n");

    if (crc != archead.crc) {
       if (warning)
           printf("File %s failed CRC check (CRC %04X)\n", archead.name, crc);
       errors++;
       return;
    }
    return;
}
