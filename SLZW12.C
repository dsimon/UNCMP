/*******************************************************************
* UNCMP - SLZW12, Version 1.20, created 3-17-90
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
#include <fcntl.h>
#if defined( __TURBOC__ )
# include <mem.h>
# include <alloc.h>
#else
# include <memory.h>
# include <string.h>
# include <malloc.h>
#endif
#include "uncmp.h"
#include "archead.h"
#include "global.h"

#define FALSE      (0)
#define TRUE       !FALSE
#define TABSIZE    4096
#define STACKSIZE  6144 /* TABSIZE */
#define NO_PRED    0xFFFF
#define EMPTY      0xF000
#define NOT_FND    0xFFFF
#define UEOF       ((unsigned)EOF)
#define UPDATE     TRUE

void setmemory(char FAR * mem, char value, unsigned long size);
void init_tab(void);
unsigned int pop(void);
void push(unsigned int);
int getcode12(FILE *);
void upd_tab(unsigned int, unsigned int);
unsigned int hash(unsigned int, unsigned char, int);

static int sp = 0;                     /* current stack pointer */

static char FAR *stack;                /* stack for pushing and popping */

 /* characters */

static struct entry {
    char used;
    unsigned int next;         /* hi bit is 'used' flag      */
    unsigned int predecessor;  /* 12 bit code                */
    unsigned char follower;
} FAR *string_tab;

int slzw_decomp(FILE * in, FILE * out, int arctype)
{
    register unsigned int c, tempc;
    unsigned int code, oldcode, incode, finchar, lastchar;
    char unknown = FALSE;
    int code_count = TABSIZE - 256;
    struct entry FAR *ep;

    sp = 0;

    if ((string_tab = (struct entry FAR *) fmalloc(sizeof(struct entry) * TABSIZE)) == NULL)
       mem_error();
    if ((stack = (char FAR *) fmalloc(sizeof(char) * STACKSIZE)) == NULL)
       mem_error();

    headertype = arctype;

    init_tab();                        /* set up atomic code definitions */
    code = oldcode = getcode12(in);
    c = string_tab[code].follower;     /* first code always known      */
    if (headertype == 5) {
       add1crc(c);
       putc(c, out);
    } else
       putc_rle(c, out);
    finchar = c;

    while (UEOF != (code = incode = getcode12(in))) {
       ep = &(string_tab[code]);       /* initialize pointer           */
       if (!ep->used) {        /* if code isn't known          */
           lastchar = finchar;
           code = oldcode;
           unknown = TRUE;
           ep = &(string_tab[code]);   /* re-initialize pointer        */
       }
       while (NO_PRED != ep->predecessor) {

           /* decode string backwards into stack */
           push(ep->follower);

           code = ep->predecessor;
           ep = &(string_tab[code]);
       }

       finchar = ep->follower;

       /* above loop terminates, one way or another, with                  */
       /* string_tab[code].follower = first char in string                 */

       if (headertype == 5) {
           add1crc(finchar);
           putc(c, out);
       } else
           putc_rle(finchar, out);

       /* pop anything stacked during code parsing                         */

       while (EMPTY != (tempc = pop())) {
           if (headertype == 5) {
               add1crc(tempc);
               putc(tempc, out);
           } else
               putc_rle(tempc, out);
       }
       if (unknown) {          /* if code isn't known the follower char of
                                * last */
           if (headertype == 5) {
               finchar = lastchar;
               add1crc(finchar);
               putc(finchar, out);
           } else
               putc_rle(finchar = lastchar, out);
           unknown = FALSE;
       }
       if (code_count) {
           upd_tab(oldcode, finchar);
           --code_count;
       }
       oldcode = incode;
    }

    ffree(stack);
    ffree(string_tab);

    return (0);                        /* close all files and quit */
}

unsigned hash(unsigned int pred, unsigned char foll, int update)
{
    register unsigned int local, tempnext;
    static long temp;
    register struct entry FAR *ep;

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
       local = (temp >> 6) & 0x0FFF;   /* middle 12 bits of result */
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
       ep = &(string_tab[tempnext]);   /* initialize pointer   */
       while (ep->used) {
           ++tempnext;
           if (tempnext == TABSIZE) {
               tempnext = 0;   /* handle wrap to beginning of table    */
               ep = string_tab;/* address of first element of table    */
           } else
               ++ep;           /* point to next element in table       */
       }

       /* put new tempnext into last element in collision list             */

       if (update)             /* if update requested                  */
           string_tab[local].next = tempnext;
       return tempnext;
    }
}

void init_tab(void)
{
    register unsigned int i;

    setmemory((char FAR *) string_tab, (char) 0, (unsigned long) sizeof(struct entry) * TABSIZE);
    for (i = 0; i <= 255; i++) {
       upd_tab(NO_PRED, i);
    }
}

void upd_tab(unsigned int pred, unsigned int foll)
{
    struct entry FAR *ep;      /* pointer to current entry */

    /* calculate offset just once */
    ep = &(string_tab[hash(pred, foll, UPDATE)]);
    ep->used = TRUE;
    ep->next = 0;
    ep->predecessor = pred;
    ep->follower = foll;
}

/* getcode fills an input buffer of bits and returns the next 12 bits */
/* from that buffer with each call */

int getcode12(FILE * in)
{
    register int localbuf, returnval;
    static unsigned int inbuf = EMPTY;

    if (EMPTY == inbuf) {      /* On code boundary */
       if ((sizeleft - 2) < 0)
           return EOF;
       sizeleft -= 2;
       localbuf = getc(in);
       localbuf &= 0xFF;
       inbuf = getc(in);
       inbuf &= 0xFF;
       returnval = ((localbuf << 4) & 0xFF0) + ((inbuf >> 4) & 0x00F);
       inbuf &= 0x000F;
    } else {                   /* buffer contains nibble H */
       if (!sizeleft)
           return EOF;
       sizeleft--;
       localbuf = getc(in);
       localbuf &= 0xFF;
       returnval = localbuf + ((inbuf << 8) & 0xF00);
       inbuf = EMPTY;
    }
    return returnval;
}

void setmemory(char FAR * mem, char value, unsigned long size)
{
    register unsigned long i;

    for (i = 0; i < size; i++)
       mem[i] = value;
}

void push(unsigned int c)
{
  stack[sp] = ((char) c);     /* coerce passed integer into a character */
  ++sp;
  if (sp >= STACKSIZE) {
    fprintf(stderr,"stack overflow in SLZW, aborting\n");
    exit(1);            /* non-0 exit parameter terminates batch job    */
    }
  }

unsigned int pop(void)
{
  if (sp > 0) {
    --sp;               /* push leaves sp pointing to next empty slot   */
    return ( (int) stack[sp] ); /* make sure pop returns char           */
    }
  else
    return EMPTY;
}


