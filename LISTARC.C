/*******************************************************************
* UNCMP - LISTARC, Version 1.20, created 3-17-90
*
* Archive directory lister.
*
* Lists contents of archives in standard verbose style.
*
* The great majority of this code came from AV v2.01 by Derron
* Simon, of which the source code has not been released.
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
#include <string.h>
#include "uncmp.h"
#include "archead.h"
#include "global.h"

void list_arc(FILE * in)
{
    unsigned long int total_size = 0L;
    unsigned long total_length = 0L;
    unsigned int total_files = 0;
    char method[9];
    char crushing = 0;
    int hour, min, sec;
    int month, day, year;

    printf("  Filename      Length   Method   SF     Size   Date     Time    CRC\n"
    "  --------      ------   ------   --     ----   ----     ----    ---\n");

    do {
         switch ((int) archead.atype) {
         case 1:
              strcpy(method, " stored ");
         case 2:
              strcpy(method, " Stored ");
              break;
         case 3:
              strcpy(method, " Packed ");
              break;
         case 4:
              strcpy(method, "Squeezed");
              break;
         case 5:
         case 6:
         case 7:
              strcpy(method, "crunched");
              break;
         case 8:
              strcpy(method, "Crunched");
              break;
         case 9:
              strcpy(method, "Squashed");
              break;
         case 10:
              strcpy(method, "Crushed*");
              crushing++;
              break;
         case 11:
              strcpy(method, "Distill*");
              crushing++;
              break;
         default:
              strcpy(method, "Unknown!");
         }

         year = (archead.date >> 9) & 0x7f;      /* dissect the date */
         month = (archead.date >> 5) & 0x0f;
         day = archead.date & 0x1f;

         hour = (archead.time >> 11) & 0x1f;     /* dissect the time */
         min = (archead.time >> 5) & 0x3f;
         sec = (archead.time & 0x1f) * 2;

         if (check_list(archead.name)) {
              printf("  %-12s %7lu  %8s  %02d%% %7lu %2d-%02d-%2d %02d:%02d:%02d %04X\n", archead.name, archead.length, method,
              calcsf(archead.length, archead.size), archead.size, month, day, year + 80, hour, min, sec, archead.crc);
              total_length += archead.length;
              total_size += archead.size;
              total_files++;
         }
         fseek(in, archead.size, 1);
    }
    while (getarcheader(in) != 0);
    printf("  --------      ------            --     ----\n");
    printf("  Total: %-3d   %7lu            %02d%% %7lu\n", total_files, total_length, calcsf(total_length, total_size), total_size);
    if (crushing > 0)
         printf("\n* Crushing and Distilling not supported in this version of UNCMP\n");
    fclose(in);
}

int calcsf(long length_now, long org_size)
{
    if (length_now == 0)
         return 0;               /* avoid divide-by-zero error */

    /* divide in fixed point, to avoid FP */

    return ((int) (100 - ((org_size * 100) / (length_now))));
}
