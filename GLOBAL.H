/*******************************************************************
* UNCMP - GLOBAL.H, Version 1.20, created 3-17-90
*
* Externs for all global variables used by various modules
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

extern unsigned int state;
extern unsigned int crc;
extern long sizeleft;
extern int errors;
extern char path[63];
extern int crctab[];
extern int headertype;

extern struct archive_header archead;

extern char warning;
extern char overwrite;
extern char testinteg;
extern char listarchive;
extern char console;
extern char extractnew;