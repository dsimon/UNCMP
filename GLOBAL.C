/*******************************************************************
* UNCMP - GLOBAL, Version 1.04, created 7-03-89
*
* Contains all global variables used by the modules.
*
* Every variable accessed by more than one module is here.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include "uncmp.h"
#include "archead.h"

unsigned char NEAR state;         /* state of ncr packing */
unsigned int NEAR crc;            /* crc of current file */
long NEAR sizeleft;               /* for fileio routines */
int errors=0;                     /* number of errors */
char path[63];                    /* path name to output to */
char NEAR headertype;             /* headertype of archive */

struct archive_header archead;    /* header for current archive */

/* command line switches (flags) */

char warning=1;
char overwrite=0;
char testinteg=0;
char listarchive=0;

