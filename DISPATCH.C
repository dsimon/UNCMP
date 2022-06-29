/*******************************************************************
* UNCMP - DISPATCH, Version 1.03, created 6-28-89
*
* Uncompresses files by compression type.
*
* Determines archive uncompression method from archive header.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "archead.h"
#include "global.h"
#include "uncmp.h"

int uncmp(FILE *in, FILE *out)
{
    crc = 0;
    sizeleft = archead.size;
    state = 0;          /* set to NOHIST for rle packing */

    switch(archead.atype) {
         case 1:
         case 2:
              if (archead.atype == 1) printf("Unstoring,    ");
              else printf("UnStoring,    ");
              store_decomp(in,out);
              break;
         case 3:
              printf("UnPacking,    ");
              rle_decomp(in,out);
              break;
         case 4:
              printf("UnSqueezing,  ");
              sq_decomp(in,out);
              break;
         case 5:
         case 6:
         case 7:
              printf("Uncrunching,  ");
              slzw_decomp(in,out,archead.atype);
              break;
         case 8:
         case 9:
              if (archead.atype == 8) printf("UnCrunching,  ");
              else printf("UnSquashing,  ");
              dlzw_decomp(in,out,archead.atype,archead.size);
              break;
         case 10:
              if (warning) printf("\nCrushing not supported in this version of UNCMP, skipping\n");
              fseek(in,archead.size,1);
              return(1);
         default:
              if (warning) {
                   printf("\nFile uses unknown compression type,\n");
                   printf("I think you need a newer version of UNCMP\n");
              }
              fseek(in,archead.size,1);
              errors++;
              return (1);
         }

    fflush(out);
    if (ferror(out)) {
         printf("\nError writing file\nPossible disk full?\n");
         exit(1);
         }

    printf("Done\n");
    if (crc != archead.crc) {
         if (warning) printf("File %s failed CRC check (CRC %04X)\n",archead.name,crc);
         errors++;
         return (1);
         }
    return (0);
    }
