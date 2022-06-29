/*******************************************************************
* UNCMP - GLOBAL.H, Version 1.04, created 7-03-89
*
* Externs for all global variables used by various modules
*
* This code has been released into the Public Domain.
*******************************************************************/

extern unsigned char NEAR state;
extern unsigned int NEAR crc;
extern long sizeleft;
extern int errors;
extern char path[63];
extern int NEAR crctab[];
extern char NEAR headertype;

extern struct archive_header archead;

extern char warning;
extern char overwrite;
extern char testinteg;
extern char listarchive;
