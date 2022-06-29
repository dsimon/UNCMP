/*******************************************************************
* UNCMP - ARCHEAD.H, Version 1.03, created 6-28-89
*
* Definition of archive header structure.
*
* This code has been released into the Public Domain.
*******************************************************************/

#pragma pack(1)

struct archive_header {
    char            arcmark; /* arc mark = 0x1a */
    char            atype;   /* header version 0 = end, else pack
                              * method */
    char            name[13];/* file name */
    unsigned long   size;    /* size of compressed file */
    short           date;    /* file date */
    short           time;    /* file time */
    unsigned short  crc;     /* cyclic redundancy check */
    unsigned long   length;  /* true file length */
};                           /* the next size bytes after the header */
#pragma pack()
