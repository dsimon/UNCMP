/*******************************************************************
* UNCMP - DISPATCH, Version 1.20, created 3-17-90
*
* Uncompresses files by compression type.
*
* Determines archive uncompression method from archive header.
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

int uncmp(FILE * in, FILE * out)
{
    switch (archead.atype) {
       case 1:
           printf("Unstoring,    ");
           break;
       case 2:
           printf("UnStoring,    ");
           break;
       case 3:
           printf("UnPacking,    ");
           break;
       case 4:
           printf("UnSqueezing,  ");
           break;
       case 5:
       case 6:
       case 7:
           printf("Uncrunching,  ");
           break;
       case 8:
           printf("UnCrunching,  ");
           break;
       case 9:
           printf("UnSquashing,  ");
           break;
       case 10:
       case 11:
           if (warning)
               printf("\nCrushing and Distilling not supported in this version of UNCMP, skipping\n");
           fseek(in, archead.size, 1);
           return (1);
       default:
           if (warning) {
               printf("\nFile uses unknown compression type,\n");
               printf("I think you need a newer version of UNCMP\n");
           }
           fseek(in, archead.size, 1);
           errors++;
           return (1);
    }

    douncmp(in, out);        /* actually dispatch the compression */

    fflush(out);
    if (ferror(out)) {
       printf("\nError writing file\nPossible disk full?\n");
       exit(1);
    }
    printf("Done\n");
    if (crc != archead.crc) {
       if (warning)
           printf("File %s failed CRC check (CRC %04X)\n", archead.name, crc);
       errors++;
       return (1);
    }
    return (0);
}
