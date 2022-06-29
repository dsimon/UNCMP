/*******************************************************************
* UNCMP - GETHEAD, Version 1.20, created 3-17-90
*
* Archive header reader
*
* Reads the header of archives (including incompatible type 1).
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

int getarcheader(FILE * in)
{
    /* read in archead minus the length field */

    if ((fread(&archead, sizeof(char), sizeof(struct archive_header) - sizeof(long), in)) < 2) {
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
       return (0);

    /* if not obsolete header type then the next long is the length field */

    if (archead.atype != 1) {
       if ((fread(&archead.length, sizeof(long), 1, in)) < 1) {
           printf("Archive has invalid header\n");
           exit(1);
       }
    }
    /* if obsolete then set length field equal to size field */

    else
       archead.length = archead.size;

    return (1);
}
