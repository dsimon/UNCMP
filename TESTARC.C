/*******************************************************************
* UNCMP - TESTARC, Version 1.20, created 3-17-90
*
* Test for corrupt archives by taking CRC of files included.
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
#include <string.h>
#include "uncmp.h"
#include "archead.h"
#include "global.h"

int testarc(FILE * in)
{
    FILE *null;

    crc = 0;
    sizeleft = archead.size;
    state = 0;                 /* NOHIST */

    if ((null = fopen("NUL", "wb")) == NULL) {
       printf("Error opening NUL device\n");
       exit(1);
    }

    switch (archead.atype) {
       case 1:
       case 2:
           if (archead.atype == 1)
               printf("Unstoring,    ");
           else
               printf("UnStoring,    ");
           break;
       case 3:         /* can't bypass output with RLE */
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
       case 9:
           if (archead.atype == 8)
               printf("UnCrunching,  ");
           else
               printf("UnSquashing,  ");
           break;
       case 10:
       case 11:
           if (warning)
               printf("\nCrushing and Distilling not supported in this version of UNCMP, skipping\n");
           fseek(in, archead.size, 1);
           return (1);
       default:
           if (warning)
               printf("Unknown compression type, skipping\n");
           fseek(in, archead.size, 1);
    }

    douncmp(in,null);        /* do actual uncompression to null device */

    if (crc != archead.crc) {
       printf("Failed\n");
       errors++;
    } else
       printf("Ok\n");

    fclose(null);
    return (0);
}
