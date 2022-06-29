/*******************************************************************
* UNCMP - GLOBAL, Version 1.03, created 6-28-89
*
* Contains all global variables used by the modules.
*
* Every variable accessed by more than one module is here.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include "archead.h"

unsigned char state;              /* state of ncr packing */
unsigned int crc;                 /* crc of current file */
long sizeleft;                    /* for fileio routines */
int lastc;                        /* last character ouput by putc_rle() */
int errors=0;                     /* number of errors */
char path[63];                    /* path name to output to */
char headertype;                  /* headertype of archive */

struct archive_header archead;    /* header for current archive */

/* command line switches (flags) */

char warning=1;
char overwrite=0;
char testinteg=0;
char listarchive=0;
