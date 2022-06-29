/*******************************************************************
* UNCMP - UNCMP.H, Version 1.20, created 3-17-90
*
* Function prototypes for all functions used, as well as all the
* defines that are needed for all of the modules.
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

/* defines */

#define OUTBUFSIZE      16384  /* 16k output buffer */
#define INBUFSIZE       16384  /* 16k input buffer */
#define SQUASH          1
#define CRUNCH          0

/* Defines for Turbo C Compiler version 2.0 */

#if defined(_386_)
# define NAME "UNCMP v1.20, 80386"
#elif defined(_286_)
# define NAME "UNCMP v1.20, 80286"
#else
# define NAME "UNCMP v1.20, 8088"
#endif

#if defined( __TURBOC__ )             /* if TURBO C */

# define COMPILER       "Turbo C 2.0 and TLINK 2.0"
# define fmalloc(x)     farmalloc(x)   /* fmalloc is use by MSC to allocated far */
                                       /* memory, farmalloc is the equivalent for */
                                       /* TC */
# define ffree(x)       farfree(x)     /* ffree is used by MSC to free far mem */
                                       /* farfree is the equivalent for TC */
# define PASCAL     pascal      /* improves calling speed slightly */
# define NEAR       near        /* makes commonly used data easier to reference */
# define FAR        far         /* forces 32-bit pointer for data */

#elif defined( QC ) || defined ( MSC ) /* if MSC 5.1 or Quick C 2.00 */

/* Defines for Microsoft C 5.0, 5.1, and Microsoft Quick C 1.0 and 2.0 */

# if defined( MSC )
#  define COMPILER  "MSC 5.1 and LINK 4.06"
# elif defined( QC )
#  define COMPILER  "Quick C 2.0 and LINK 4.06"
# endif
# define fmalloc(x)     _fmalloc(x)
# define ffree(x)       _ffree(x)
# define PASCAL         pascal      /* improves calling speed slightly */
# define NEAR           near        /* makes commonly used data easier to reference */
# define FAR            far         /* forces 32-bit pointer for data */

#else                           /* else generic ANSI compiler */

/* Defines for a generic ANSI compatible C compiler, see docs for details */

# define COMPILER       "ANSI C Compiler"
# define fmalloc(x)     malloc(x)
# define ffree(x)       free(x)
# define PASCAL
# define NEAR
# define FAR

#endif

/* the following is an inline version of addcrc for 1 character, implemented */
/* as a macro, it really speeds things up. */

#define add1crc(x) crc = ((crc >> 8) & 0x00FF) ^ crctab[(crc ^ x) & 0x00FF];

/* function prototypes */

void main(int argc, char **argv);
void addcrc(char *cc, int i);
void dlzw_decomp(FILE * in, FILE * out, int arctype);
void putc_rle(unsigned char c, FILE * out);
int getarcheader(FILE * in);
void sq_decomp(FILE * in, FILE * out);
void list_arc(FILE * in);
int calcsf(long length_now, long org_size);
void extract_file(FILE * in, char *filename);
void help(void);
void test_file(FILE * in, char *filename);
int slzw_decomp(FILE * in, FILE * out, int arctype);
int testarc(FILE * in);
int uncmp(FILE * in, FILE * out);
char *setup_name(char *filename);
void setup_list(char *filename);
int check_list(char *filename);
int compare_files(char *filename, char *filespec);
int setup_path(char *filename);
void store_decomp(FILE * in, FILE * out);
void read_error(void);
void write_error(void);
void mem_error(void);
void rle_decomp(FILE * in, FILE * out);
int getcode(FILE * in);
void license(void);
void con_output(FILE * in,char *filename);
void douncmp(FILE *in, FILE *out);
