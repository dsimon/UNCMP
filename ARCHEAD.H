/*******************************************************************
* UNCMP - ARCHEAD.H, Version 1.20, created 3-17-90
*
* Definition of archive header structure.
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

#pragma pack(1)

struct archive_header {
    char arcmark;              /* arc mark = 0x1a */
    char atype;                        /* header version 0 = end, else pack method */
    char name[13];             /* file name */
    unsigned long size;                /* size of compressed file */
    int date;                  /* file date */
    int time;                  /* file time */
    unsigned crc;              /* cyclic redundancy check */
    unsigned long length;      /* true file length */
};                             /* the next size bytes after the header */

#pragma pack()
