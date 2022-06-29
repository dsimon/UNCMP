/*******************************************************************
* UNCMP - UNCMP, Version 1.20, created 3-17-90
*
* Main part of Uncompress program.
*
* Reads the command line and determines which routines to run.
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
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>
#if defined( __TURBOC__ )
# include <dir.h>
#endif
#include "uncmp.h"
#include "archead.h"
#include "global.h"

void main(int argc, char **argv)
{
    FILE *in;
    char arcname[80];
    int curarg = 1;           /* current argument in arg list */

    printf(NAME " - Archive UnCompressor, Copyright (c) 1989-1990 Derron Simon\n");
    printf("Compiled on " __DATE__ " with " COMPILER "\n\n");

    while ((argv[curarg][0] == '/') || (argv[curarg][0] == '-')) {
         switch (tolower(argv[curarg][1])) {
              case 'e':
              case 'x':           /* extract */
                   break;
              case 'p':           /* output to console */
                   console = 1;
                   break;
              case 'w':           /* warnings off */
                   warning = 0;
                   break;
              case 'o':           /* overwrite on */
                   overwrite = 1;
                   break;
              case 't':           /* test integrity */
                   testinteg = 1;
                   break;
              case 'l':
              case 'v':           /* verbose listing of archive */
                   listarchive = 1;
                   break;
              case 'h':
              case '?':           /* help with UNCMP */
                   help();
                   exit(1);
              case 'c':
                   license();
                   exit(0);
              case 'n':           /* extract only newer files */
                   extractnew = 1;
                   overwrite = 1; /* extractnew implies overwrite */
                   break;
              default:
                   printf("Unknown option, run UNCMP without arguments for help\n");
                   exit(1);
              }
         curarg++;
         }

    if (argc < 2 || curarg == argc) {
       help();
       exit(1);
    }

    strcpy(arcname, strupr(argv[curarg++]));
    if (!strchr(arcname, '.'))
       strcat(arcname, ".ARC");

    /* if path is the next argument then curarg++, else let it be */

    if (setup_path(strupr(argv[curarg])))
       curarg++;

    /* put the arguments in the filename list */

    while (curarg != argc) {
       setup_list(argv[curarg]);
       curarg++;
    }

    if ((in = fopen(arcname, "rb")) == NULL) {
       printf("Can't open archive %s\n", arcname);
       exit(1);
    }

    /* setvbuf must be called immediately after fopen */

    if (setvbuf(in, NULL, _IOFBF, INBUFSIZE)) {
         mem_error();
         }

    if (!getarcheader(in)) {
         read_error();
         }

    do {                       /* MAIN LOOP */
       if (testinteg) {
           if (check_list(archead.name))       /* if match then test file */
               test_file(in, archead.name);
           else
               fseek(in, archead.size, 1);     /* else skip it */
       } else if (listarchive) {
           list_arc(in);       /* list_arc() only gets */
           break;              /* called once */
       } else if (console) {
           if (check_list(archead.name))
               con_output(in, strupr(archead.name));
           else
               fseek(in, archead.size, 1);
       } else {
           if (check_list(archead.name))       /* if match then extract file */
               extract_file(in, archead.name);
           else
               fseek(in, archead.size, 1);     /* else skip it */
       }
    } while (getarcheader(in));
    fclose(in);
    if (testinteg)
       printf("%d error(s) detected\n", errors);
    printf("\nUNCMP complete\n");
    exit(errors);
}

/* this function extracts a file and tests to see if duplicate exists */

void extract_file(FILE * in, char *filename)
{

#if defined( __TURBOC__ )
    struct ffblk buffer;
#elif defined( QC ) || defined( MSC )
    struct find_t buffer;
#endif

    char outfile[80];
    int reply;
    FILE *out;
    int failure = 0;

    /* create filename with specified path */

    strcpy(outfile, path);
    strcat(outfile, filename);

    if (extractnew) {
#if defined( __TURBOC__ )
         if (!findfirst(outfile, &buffer, FA_ARCH)) {
#elif defined ( QC ) || defined ( MSC )
         if (!_dos_findfirst(outfile, _A_NORMAL, &buffer)) {
#endif
#if defined( __TURBOC__ ) || defined( QC ) || defined( MSC )
              if ((archead.date < buffer.wr_date) || (archead.date == buffer.wr_date && archead.time < buffer.wr_time)) {
                   printf("Skipping file: %s\n",archead.name);
                   fseek(in, archead.size, 1);
                   return;
                   }
              }
         }
#endif

    if (!overwrite) {
#if defined( __TURBOC__ )
         if (!findfirst(outfile, &buffer, FA_ARCH)) {
#elif defined( QC ) || defined ( MSC )
         if (!_dos_findfirst(outfile, _A_NORMAL, &buffer)) {
#endif
#if defined( __TURBOC__ ) || defined( QC ) || defined( MSC )
              printf("File %s exists, overwrite(Y/N/A)? ", archead.name);
              fflush(stdin);
              reply = getc(stdin);
              if ((reply == 'A') || (reply == 'a'))
                   overwrite = 1;
              else if ((reply != 'y') && (reply != 'Y')) {
                   printf("Skipping file: %s\n", archead.name);
                   fseek(in, archead.size, 1);
                   return;
                   }
              }
#endif
         }

    if ((out = fopen(outfile, "w+b")) == NULL) {
       printf("Cannot create file %s\n", outfile);
       exit(1);
    }
    if (setvbuf(out, NULL, _IOFBF, OUTBUFSIZE)) {
       mem_error();
    }
    rewind(out);

    printf("Extracting file: %-15s ", strupr(filename));
    failure = uncmp(in, out);
    fflush(out);

    /* set date and time, but skip if not MSC since Turbo C has no */
    /* equivalent function */

#if defined( QC ) || defined ( MSC )
    _dos_setftime(fileno(out), archead.date, archead.time);
#endif

    fclose(out);

    /* if errors during uncompression, than delete attempt at uncompression */

    if (failure) {
         unlink(outfile);
         }
    }

void help(void)
{
    printf("UNCMP uncompresses archives created with ARC-compatible archivers\n\n");
    printf("Usage: uncmp -[xtvcp] -[won] archive[.ext] [d:\\path\\] [filename.ext] [...]\n");
    printf("  - \"[xtvcp]\" specifies operation\n");
    printf("    - \"-x\" extract files (default)\n");
    printf("    - \"-t\" test archive integrity\n");
    printf("    - \"-v\" list contents of archive\n");
    printf("    - \"-c\" show conditions of program use (license)\n");
    printf("    - \"-p\" extract files to console\n");
    printf("  - \"[won]\" specifies option\n");
    printf("    - \"-w\" suppresses warnings\n");
    printf("    - \"-o\" overwrite existing files\n");
    printf("    - \"-n\" extract only newer files\n");
    printf("  - \"archive.ext\" is the archive to extract from with optional extension,\n");
    printf("    ARC is assumed if none is specified.\n");
    printf("  - \"d:\\path\\\" specifies the drive and/or path to extract files to.\n");
    printf("  - \"filename.ext\" is the name of a file to extract, any number can be\n");
    printf("    specified.\n");
}

/* the same as extract except it doesn't check for already existing files */
/* and doesn't setup buffers */

void test_file(FILE * in, char *filename)
{
    printf("Testing file: %-15s ", strupr(filename));
    testarc(in);
}

