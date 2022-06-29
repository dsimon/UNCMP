/*******************************************************************
* UNCMP - SLZW12, Version 1.03, created 6-28-89
*
* Static Lempel-Ziv-Welch 12 bit uncompression module.
*
* Uncompresses files created using the obsolete compression methods
* 5 (12 bit compression without RLE), 6 (12 bit compression with
* RLE), and 7 (12 bit compression with RLE and new hash method).
* The basic method is FC-FREEZE (See Storer, James A., _Data
* Compression Method and Theory_, 1989, Computer Science Press).
*
* The great majority of this code came from LZWUNC.C by Kent
* Williams which is in the Public Domain.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#ifdef __TURBOC__
# include <mem.h>
#else /* MSC */
# include <memory.h>
# include <string.h>
#endif
#include "archead.h"
#include "global.h"
#include "uncmp.h"

#define FALSE      (0)
#define TRUE       !FALSE
#define TABSIZE    4096
#define STACKSIZE  TABSIZE
#define NO_PRED    0xFFFF
#define EMPTY      0xF000
#define NOT_FND    0xFFFF
#define UEOF       ((unsigned)EOF)
#define UPDATE     TRUE

void init_tab(void);
int getcode12(FILE *);
void upd_tab(unsigned int, unsigned int);
unsigned int hash(unsigned int, unsigned char, int);

static char stack[STACKSIZE];     /* stack for pushing and popping */
                                  /* characters */
static int sp = 0;                /* current stack pointer */
static unsigned int inbuf = EMPTY;

static struct entry {
    char used;
    unsigned int next;            /* hi bit is 'used' flag      */
    unsigned int predecessor;     /* 12 bit code                */
    unsigned char follower;
} string_tab[TABSIZE];

int slzw_decomp(FILE * in, FILE * out, int arctype)
{
    register unsigned int c, tempc;
    unsigned int code, oldcode, incode, finchar, lastchar;
    char unknown = FALSE;
    int code_count = TABSIZE - 256;
    struct entry *ep;

    headertype = arctype;

    init_tab();              /* set up atomic code definitions */
    code = oldcode = getcode12(in);
    c = string_tab[code].follower;     /* first code always known      */
    if (headertype == 5)
         putc_pak(c, out);
    else
         putc_rle(c, out);
    finchar = c;

    while (UEOF != (code = incode = getcode12(in))) {
    ep = &string_tab[code];  /* initialize pointer           */
    if (!ep->used) {         /* if code isn't known          */
        lastchar = finchar;
        code = oldcode;
        unknown = TRUE;
        ep = &string_tab[code];   /* re-initialize pointer        */
    }
    while (NO_PRED != ep->predecessor) {

         /* decode string backwards into stack */

         stack[sp++] = (char)ep->follower;
         if (sp >= STACKSIZE) {
              printf("\nStack overflow, aborting\n");
              exit(1);
              }

        code = ep->predecessor;
        ep = &string_tab[code];
    }

    finchar = ep->follower;

    /* above loop terminates, one way or another, with                  */
    /* string_tab[code].follower = first char in string                 */

    if (headertype == 5)
        putc_pak(finchar, out);
    else
        putc_rle(finchar, out);

    /* pop anything stacked during code parsing                         */

    while (EMPTY != (tempc = (sp > 0) ? (int)stack[--sp] : EMPTY)) {
         if (headertype == 5)
              putc_pak(tempc, out);
         else
              putc_rle(tempc, out);
    }
    if (unknown) {      /* if code isn't known the follower char of last */
         if (headertype == 5)
              putc_pak(finchar = lastchar, out);
         else
              putc_rle(finchar = lastchar, out);
        unknown = FALSE;
    }
    if (code_count) {
        upd_tab(oldcode, finchar);
        --code_count;
    }
    oldcode = incode;
    }
    return (0);              /* close all files and quit */
}

unsigned hash(unsigned int pred, unsigned char foll, int update)
{
    register unsigned int local, tempnext;
    static long temp;
    register struct entry *ep;

    if (headertype == 7)

    /* I'm not sure if this works, since I've never seen an archive with */
    /* header type 7.  If you encounter one, please try it and tell me   */

         local = ((pred + foll) * 15073) & 0xFFF;
    else {

    /* this uses the 'mid-square' algorithm. I.E. for a hash val of n bits  */
    /* hash = middle binary digits of (key * key).  Upon collision, hash    */
    /* searches down linked list of keys that hashed to that key already.   */
    /* It will NOT notice if the table is full. This must be handled        */
    /* elsewhere.                                                           */

         temp = (pred + foll) | 0x0800;
         temp *= temp;
         local = (temp >> 6) & 0x0FFF; /* middle 12 bits of result */
         }

    if (!string_tab[local].used)
    return local;
    else {

         /* if collision has occured */

         /* a function called eolist used to be here.  tempnext is used */
         /* because a temporary variable was needed and tempnext in not */
         /* used till later on. */

         while (0 != (tempnext = string_tab[local].next))
              local = tempnext;

         /* search for free entry from local + 101 */

         tempnext = (local + 101) & 0x0FFF;
         ep = &string_tab[tempnext];   /* initialize pointer   */
         while (ep->used) {
              ++tempnext;
              if (tempnext == TABSIZE) {
                   tempnext = 0;  /* handle wrap to beginning of table    */
                   ep = string_tab;/* address of first element of table    */
                   } else
              ++ep;          /* point to next element in table       */
              }

         /* put new tempnext into last element in collision list             */

         if (update)         /* if update requested                  */
              string_tab[local].next = tempnext;
         return tempnext;
         }
    }

void init_tab(void)
{
    register unsigned int i;

    memset((char *) string_tab, 0, sizeof(string_tab));
    for (i = 0; i <= 255; i++) {
         upd_tab(NO_PRED, i);
         }
}

void upd_tab(unsigned int pred, unsigned int foll)
{
    struct entry *ep;    /* pointer to current entry */

    /* calculate offset just once */
    ep = &string_tab[hash(pred, foll, UPDATE)];
    ep->used = TRUE;
    ep->next = 0;
    ep->predecessor = pred;
    ep->follower = foll;
}

/* getcode fills an input buffer of bits and returns the next 12 bits */
/* from that buffer with each call */

int getcode12(FILE *in)
{
    register int localbuf, returnval;

    if (EMPTY == inbuf) {    /* On code boundary */
         if (EOF == (localbuf = getc_pak(in))) { /* H L1 byte - on code boundary */
              return EOF;
              }
         localbuf &= 0xFF;
         if (EOF == (inbuf = getc_pak(in))) {    /* L0 Hnext */
              return EOF;   /* The file should always end on code boundary */
              }
         inbuf &= 0xFF;
         returnval = ((localbuf << 4) & 0xFF0) + ((inbuf >> 4) & 0x00F);
         inbuf &= 0x000F;
    } else {                 /* buffer contains nibble H */
         if (EOF == (localbuf = getc_pak(in)))
              return EOF;
         localbuf &= 0xFF;
         returnval = localbuf + ((inbuf << 8) & 0xF00);
         inbuf = EMPTY;
         }
    return returnval;
}