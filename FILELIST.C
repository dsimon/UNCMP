/*******************************************************************
* UNCMP - FILELIST, Version 1.04, created 7-03-89
*
* Create a linked list of file and check for matches
*
* Creates a singly-linked list of filenames after qualifying them
* and then takes filenames and searches for matches in the list.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifdef __TURBOC__
# include <alloc.h>
#else /* MSC */
# include <malloc.h>
#endif
#include <string.h>
#include "uncmp.h"
#include "archead.h"
#include "global.h"

#define TRUE  1
#define FALSE 0

struct list {
    char filespec[13];
    struct list *next;
    };

static struct list *header = NULL;     /* NULL for first initialization */
static struct list *temp;

/* makes any filename into a valid filename for use within an archive */

char *setup_name(char *filename)
{
    unsigned char namelen;
    char *strptr;

    namelen = strlen(filename);

    /* convert all forward slashes to back slashes */

    while((strptr = strchr(filename,'/')) != NULL)
         *strptr = '\\';

    /* remove trailing period */

    if (filename[namelen-1] == '.')
         filename[--namelen] = '\0';

    /* remove everything before first back slash */

    if ((strptr = strrchr(filename,'\\')) != NULL)
         filename = strptr+1;

    /* remove everything before colon */

    if ((strptr = strrchr(filename,':')) != NULL)
         filename = strptr+1;

    return(strupr(filename));
    }

/* adds filename to a singly-linked list */

void setup_list(char *filename)
{
    /* first time initialization */

    if (header == NULL) {
         if ((header = (struct list *)calloc(1,sizeof(struct list))) == NULL) {
              mem_error();
              }
         temp = header;
         }

    /* initialize new node in list */

    if ((temp->next = (struct list *)calloc(1,sizeof(struct list))) == NULL) {
         mem_error();
         }

    /* add filename to list */

    temp = temp->next;
    strcpy(temp->filespec,setup_name(filename));
    temp->next = NULL;
    }

/* check to see if filename is in list of files and return TRUE if it is */
/* or FALSE if it isn't */

int check_list(char *filename)
{
    /* convert filename to one used within archives */

    filename = setup_name(filename);

    /* if never initialized than all files should be uncompressed */

    if (header == NULL) return(1);

    /* search for filename to extract */

    temp = header;

    while (temp != NULL) {
         if (compare_files(filename,temp->filespec)) return(1);
         temp = temp->next;
         }
    return(0);
}

int compare_files(char *filename, char *filespec)
{
    while (((*filename != NULL) && (*filename != '.')) || ((*filespec != NULL) && (*filespec != '.'))) {
         if (*filename != *filespec && *filespec != '?') { /* if doesnt't comp */
              if (*filespec != '*') return (0); /* and not '*' */
              else {    /* move to extension, filename matches */
                   while ((*filename != NULL) && (*filename != '.')) filename++;
                   while ((*filespec != NULL) && (*filespec != '.')) filespec++;
                   break;
                   }
              }
         else { /* they match */
              filename++;
              filespec++;
              }
         }

    if ((*filename != NULL) && (*filename == '.')) filename++; /* inc past . */
    if ((*filespec != NULL) && (*filespec == '.')) filespec++;

    while ((*filename != NULL) || (*filespec != NULL)) {
         if ((*filename != *filespec) && (*filespec != '?')) {
              if (*filespec != '*') return (0);
              else return (1);
              }
         else {
              filename++;
              filespec++;
              }
         }
    return (1);
    }

int setup_path(char *filename)
{
    char namelen;

    namelen = strlen(filename);

    if (filename[namelen-1] == '\\'
    || filename[namelen-1] == ':' || filename[namelen-1] == '/') {
         strcpy(path,filename);
         return(1);
         }
    else {
         path[0] = NULL;
         return(0);
         }
    }