/*******************************************************************
* UNCMP - STUBS, Version 1.03, created 6-28-89
*
* Contains stubs to keep LINK from including never used routines.
*
* The function _setenvp() is called by MSC startup code to setup
* the environment pointer.  Since the environment is never used by
* UNCMP I have substituted the Microsoft routine will an empty
* function.  With MSC 5.1 it saves 192 bytes.
*
* The function _nullcheck() is called by MSC shutdown code to check
* for writes to the null segment.  Since it is needed only for
* debugging and the code is pretty well debugged, I have replaced
* it with a null routine to save space.  It saves 112 bytes with
* MSC 5.1.  I think a bug in MSC causes problems with setting the
* DOS errorcode with _nullcheck(), so this one returns an error-
* level of 0 no matter what the main code sets on exit.
*
* This code has been released into the Public Domain (what a joke!).
*******************************************************************/

void _setenvp(void)
{
}

int _nullcheck(void)
{
    return 0;
}\Rogue\Monster\
else
  echo "will not over write ./stubs.c"
fi
if `test ! -s ./testarc.c`
then
echo "writing ./testarc.c"
cat > ./testarc.c << '\Rogue\Monster\'
/*******************************************************************
* UNCMP - TESTARC, Version 1.03, created 6-28-89
*
* Test for corrupt archives by taking CRC of files included.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archead.h"
#include "global.h"
#include "uncmp.h"

int testarc(FILE *in)
{
    FILE *null;

    crc = 0;
    sizeleft = archead.size;
    state = 0;     /* NOHIST */

    if ((null = fopen("NUL","wb"))==NULL) {
         printf("Error opening NUL device\n");
         exit(1);
         }

    switch(archead.atype) {
         case 1:
         case 2:
              if (archead.atype == 1) printf("Unstoring,    ");
              else printf("UnStoring,    ");
              store_decomp(in,null);
              break;
         case 3:                                 /* can't bypass output with RLE */
              printf("UnPacking,    ");
              rle_decomp(in,null);
              break;
         case 4:
              printf("UnSqueezing,  ");
              sq_decomp(in,null);
              break;
         case 5:
         case 6:
         case 7:
              printf("Uncrunching,  ");
              slzw_decomp(in,null,archead.atype);
              break;
         case 8:
         case 9:
              if (archead.atype==8) printf("UnCrunching,  ");
              else printf("UnSquashing,  ");
              dlzw_decomp(in,null,archead.atype,archead.size);
              break;
         case 10:
              if (warning) printf("\nCrushing not supported in this version of UNCMP, skipping\n");
              fseek(in,archead.size,1);
              return(1);
         default:
              if (warning) printf("Unknown compression type, skipping\n");
              fseek(in,archead.size,1);
         }
    if (crc != archead.crc) {
         printf("Failed\n");
         errors++;
         }
    else printf("Ok\n");
    return(0);
    }\Rogue\Monster\
else
  echo "will not over write ./testarc.c"
fi
if `test ! -s ./uncmp.c`
then
echo "writing ./uncmp.c"
cat > ./uncmp.c << '\Rogue\Monster\'
/*******************************************************************
* UNCMP - UNCMP, Version 1.00, created 5-13-89
*
* Main part of Uncompress program.
*
* Reads the command line and determines which routines to run.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#ifdef __TURBOC__
# include <dir.h>
#endif

#include "archead.h"
#include "global.h"
#include "uncmp.h"
#define _A_NORMAL 0

struct find_t {
 char reserved[21];
 char attrib;
 unsigned wr_time;
 unsigned wr_date;
 long size;
 char name[13];
 };
void main(int argc, char **argv)
{
    FILE *in;
    long foffset;
    char arcname[80];
    int curarg = 1;    /* current argument in arg list */

    printf("UNCMP v1.03 - Archive UnCompressor, written 1989 by Derron Simon\n");
    printf("Compiled on " __DATE__ " with " COMPILER "\n\n");

    if (argc < 2) {
         help();
         exit(1);
         }

    while ((argv[curarg][0] == '/') || (argv[curarg][0] == '-')) {
         switch (tolower(argv[curarg][1])) {
              case 'e':
              case 'x':                /* extract */
                   break;
              case 'w':                /* warnings off */
                   warning = 0;
                   break;
              case 'o':                /* overwrite on */
                   overwrite = 1;
                   break;
              case 't':                /* test integrity */
                   testinteg = 1;
                   break;
              case 'l':
              case 'v':                /* verbose listing of archive */
                   listarchive = 1;
                   break;
              case 'h':
              case '?':                /* help with UNCMP */
                   help();
                   exit(1);
              default:
                   printf("Unknown option, run uncmp without arguments for help\n");
                   exit(1);
              }
         curarg++;
         }

    strcpy(arcname,argv[curarg++]);


    if (!strchr(arcname,'.')) strcat(arcname,".arc");

    /* if path is the next argument then curarg++, else let it be */

    if (setup_path(strupr(argv[curarg]))) curarg++;

    /* put the arguments in the filename list */

    while (curarg != argc) {
         setup_list(argv[curarg]);
         curarg++;
         }

    if ((in = fopen(arcname,"r")) == NULL) {
         printf("Can't open archive %s\n",arcname);
         exit(1);
         }

    if (!getarcheader(in)) {
         read_error();
         }

    /* what follows is a kludge to get around what could be a bug in setvbuf() */
    /* after setvbuf() the file pointer is incremented by one, so you have */
    /* to save your place before setvbuf() and restore it afterwords */

    fgetpos(in,&foffset);         /* get position */
    if (setvbuf(in,NULL,_IOFBF,INBUFSIZE)) {
         mem_error();
         }
    fflush(in);
    fsetpos(in,&foffset);         /* restore position */

    do { /* MAIN LOOP */
         if (testinteg) {
              if (check_list(archead.name))      /* if match then test file */
                   test_file(in,archead.name);
              else fseek(in,archead.size,1);     /* else skip it */
              }
         else if (listarchive) {
              list_arc(in);                      /* list_arc() only gets */
              break;                             /* called once */
              }
         else {
              if (check_list(archead.name))      /* if match then extract file */
                   extract_file(in,archead.name);
              else fseek(in,archead.size,1);     /* else skip it */
              }
         } while (getarcheader(in));
    fclose(in);
    if (testinteg) printf("%d error(s) detected\n",errors);
    printf("\nUNCMP complete\n");
    exit(errors);
    }

/* this function extracts a file and tests to see if duplicate exists */

void extract_file(FILE *in,char *filename)
{
#ifdef __TURBOC__
    struct ffblk buffer;
#else /* MSC 5.1 */
    struct find_t buffer;
#endif
    char outfile[80];
    int reply;
    FILE *out;
    int failure = 0;

    /* create filename with specified path */

    strcpy(outfile,path);
    strcat(outfile,filename);

    if (!overwrite) {
#ifdef __TURBOC__
         if (!findfirst(outfile,&buffer,FA_ARCH)) {
#else /* MSC 5.1 */
         if (!_dos_findfirst(outfile,_A_NORMAL,&buffer)) {
#endif
              printf("File %s exists, overwrite? ",archead.name);
              fflush(stdin);
              reply=getc(stdin);
              if ((reply != 'y') && (reply != 'Y')) {
                   printf("Skipping file %s\n",archead.name);
                   fseek(in,archead.size,1);
                   return;
                   }
              }
         }

    if ((out = fopen(strupr(outfile),"w")) == NULL) {
         printf("Cannot create file %s\n",outfile);
         exit(1);
         }
    if (setvbuf(out,NULL,_IOFBF,OUTBUFSIZE)) {
         mem_error();
         }
    rewind(out);

    printf("Extracting file: %-15s ",strupr(filename));
    failure = uncmp(in,out);
    fflush(out);

    /* set date and time, but skip if not MSC since Turbo C has no */
    /* equivalent function */
/*
#ifndef __TURBOC__
    _dos_setftime(fileno(out),archead.date,archead.time);
#endif
*/

    fclose(out);
#ifdef M_XENIX
 xen_setftime(strupr(outfile), archead.date, archead.time);
#endif

    /* if errors during uncompression, than delete attempt at uncompression */

    if (failure) {
         unlink(outfile);
         }
}

void help(void)
{
    printf("UNCMP uncompresses archives created with ARC-compatible archivers\n\n");
    printf("Usage: uncmp -[xtv] -[wo] archive[.ext] [path/] [filename.ext] [...]\n");
    printf("  - \"[xtv]\" specifies operation\n");
    printf("    - \"-x\" extract files (default)\n");
    printf("    - \"-t\" test archive integrity\n");
    printf("    - \"-v\" list contents of archive\n");
    printf("  - \"[wo]\" specifies option\n");
    printf("    - \"-w\" suppresses warnings\n");
    printf("    - \"-o\" overwrite existing files\n");
    printf("  - \"archive.ext\" is the archive to extract from with optional extension,\n");
    printf("    arc is assumed if none is specified.\n");
    printf("  - path/\" specifies the path to extract files to.\n");
    printf("  - \"filename.ext\" is the name of a file to extract, any number can be\n");
    printf("    specified.\n");
    }

/* the same as extract except it doesn't check for already existing files */
/* and doesn't setup buffers */

void test_file(FILE *in, char *filename)
{
    printf("Testing file: %-15s ",strupr(filename));
    testarc(in);
}
fgetpos(in, addr)
FILE *in;
long *addr;
{
 *addr = ftell(in);
}
fsetpos(in, addr)
FILE *in;
long *addr;
{
 (void)fseek(in, *addr, 0);
}
_dos_findfirst(ofile, ints, bufs )
char *ofile;
int ints;
char *bufs;
{
 int result;
 FILE *Result;
 Result = fopen(ofile, "r");
 if( Result == NULL )
  result = 1;
 else
  result = 0;
 return(result);
}

xen_setftime(f_name, f_date, f_time)  /* set a file's date/time stamp */
char  *f_name;  /* file to set stamp on */
unsigned short f_date, f_time;  /* desired date, time */
{
 time_t  times[2];
 unsigned long year, month, day, hour, minute, second;

 static unsigned long monthdays[12] = {
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
 };

 year = (f_date >> 9) + 1980;
 month = (f_date >> 5 & 0xF) - 1;
 day = f_date & 0x1F;
 hour = f_time >> 11;
 minute = f_time >> 5 & 0x3F;
 second = f_time << 1 & 0x3F;

 times[0] = time(NULL);
 times[1] = ((((year - 1970l) * 365l + ((year - 1969l) >> 2) +
     monthdays[month] + ((month > 1l && !(year & 3l)) ? 1l : 0l) +
     day - 1) * 24l + hour) * 60l + minute) * 60l + second;   
 utime(f_name, times);
}
