/*******************************************************************
* UNCMP - UNCMP.H, Version 1.03, created 6-28-89
*
* Function prototypes for all functions used, as well as all the
* defines that are needed for all of the modules.
*
* This code has been released into the Public Domain.
*******************************************************************/

/* defines */

#define OUTBUFSIZE      16384     /* 16k output buffer */
#define INBUFSIZE       16384     /* 16k input buffer */

#ifdef __TURBOC__  /* if TURBO C */
# define COMPILER  "Turbo C 2.0 and TLINK 2.0"
#else              /* Assume MSC 5.1 */
# define COMPILER  "MSC 5.1 and LINK 3.65"
#endif

#define PASCAL     pascal    /* can be undefined if compiler doesn't support */
                             /* pascal calling.  Improves speed slightly */

/* the following is an inline version of addcrc for 1 character, implemented */
/* as a macro, it really speeds things up. */

#define add1crc(x) crc = ((crc >> 8) & 0x00FF) ^ crctab[(crc ^ x) & 0x00FF];

/* function prototypes */

void main(int argc, char **argv);
void addcrc(char *cc, int i);
int dlzw_decomp(FILE *in,FILE *out,int arctype,long size);
void putc_pak(char c,FILE *out);
void putc_rle(unsigned char c,FILE *out);
int getc_pak(FILE *in);
int getarcheader(FILE *in);
void sq_decomp(FILE *in, FILE *out);
void list_arc(FILE *in);
int calcsf(long length_now,long org_size);
void extract_file(FILE *in,char *filename);
void help(void);
void test_file(FILE *in,char *filename);
int slzw_decomp(FILE *in,FILE *out,int arctype);
int testarc(FILE *in);
int uncmp(FILE *in,FILE *out);
char *setup_name(char *filename);
void setup_list(char *filename);
int check_list(char *filename);
int compare_files(char *filename, char *filespec);
int setup_path(char *filename);
void store_decomp(FILE *in, FILE *out);
void read_error(void);
void write_error(void);
void mem_error(void);
void rle_decomp(FILE *in, FILE *out);