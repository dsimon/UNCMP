/*******************************************************************
* UNCMP - GETHEAD, Version 1.03, created 6-28-89
*
* Archive header reader
*
* Reads the header of archives (including incompatible type 1).
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "archead.h"
#include "global.h"
#include "uncmp.h"

int getarcheader(in)
FILE *in;
{
    /* read in archead minus the length field */

    if ((fread((char *)&archead, sizeof(char), sizeof(struct archive_header) - sizeof(long), in)) < 2) {
         printf("Archive has invalid header\n");
         exit(1);
         }

    /* if archead.arcmark does not have that distinctive arc identifier 0x1a */
    /* then it is not an archive */

    if (archead.arcmark != 0x1a) {
         printf("Archive has invalid header\n");
         exit(1);
         }

    /* if atype is 0 then EOF */

    if (archead.atype == 0)
         return(0);

    /* if not obsolete header type then the next long is the length field */

    if (archead.atype != 1) {
     if ((fread((char *)&archead.length, sizeof(long), 1, in)) < 1) {
              printf("Archive has invalid header\n");
              exit(1);
              }
         }

    /* if obsolete then set length field equal to size field */

    else archead.length = archead.size;

    return(1);
    }
