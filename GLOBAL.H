/*******************************************************************
* UNCMP - GLOBAL.H, Version 1.03, created 6-28-89
*
* Externs for all global variables used by various modules
*
* This code has been released into the Public Domain.
*******************************************************************/

extern unsigned char state;
extern unsigned int crc;
extern long sizeleft;
extern int lastc;
extern int errors;
extern char path[63];
extern int crctab[];

extern struct archive_header archead;

extern char warning;
extern char overwrite;
extern char testinteg;
extern char listarchive;
extern char headertype;
char *strupr();
