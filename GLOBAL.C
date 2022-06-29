/*******************************************************************
* UNCMP - GLOBAL, Version 1.20, created 3-17-90
*
* Contains all global variables used by the modules.
*
* Every variable accessed by more than one module is here.
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
#include "uncmp.h"
#include "archead.h"

unsigned int state;               /* state of ncr packing */
unsigned int crc;                 /* crc of current file */
long sizeleft;                    /* for fileio routines */
int errors = 0;                   /* number of errors */
char path[63];                    /* path name to output to */
int headertype;                   /* headertype of archive */

struct archive_header archead;    /* header for current archive */

/* command line switches (flags) */

char warning = 1;
char overwrite = 0;
char testinteg = 0;
char listarchive = 0;
char console = 0;
char extractnew = 0;
