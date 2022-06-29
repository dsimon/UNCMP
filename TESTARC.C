/*******************************************************************
* UNCMP - TESTARC, Version 1.04, created 7-03-89
*
* Test for corrupt archives by taking CRC of files included.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uncmp.h"
#include "archead.h"
#include "global.h"

int testarc(FILE *in)
{
    FILE *null;

    crc = 0;
    sizeleft = archead.size;
    state = 0;     /* NOHIST */

    if ((null = fopen("NUL","wb"))==NULL) {
         printf("Error opening NUL device\n");
         exit(1);
         }

    switch(archead.atype) {
         case 1:
         case 2:
              if (archead.atype == 1) printf("Unstoring,    ");
              else printf("UnStoring,    ");
              store_decomp(in,null);
              break;
         case 3:                                 /* can't bypass output with RLE */
              printf("UnPacking,    ");
              rle_decomp(in,null);
              break;
         case 4:
              printf("UnSqueezing,  ");
              sq_decomp(in,null);
              break;
         case 5:
         case 6:
         case 7:
              printf("Uncrunching,  ");
              slzw_decomp(in,null,archead.atype);
              break;
         case 8:
         case 9:
              if (archead.atype==8) printf("UnCrunching,  ");
              else printf("UnSquashing,  ");
              dlzw_decomp(in,null,archead.atype);
              break;
         case 10:
              if (warning) printf("\nCrushing not supported in this version of UNCMP, skipping\n");
              fseek(in,archead.size,1);
              return(1);
         default:
              if (warning) printf("Unknown compression type, skipping\n");
              fseek(in,archead.size,1);
         }
    if (crc != archead.crc) {
         printf("Failed\n");
         errors++;
         }
    else printf("Ok\n");
    return(0);
    }