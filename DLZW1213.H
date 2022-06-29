/*******************************************************************
* UNCMP - DLZW1213, Version 1.03, created 6-28-89
*
* Dynamic Lempel-Ziv-Welch 12/13 bit uncompression module.
*
* Uncompresses files stored with Crunching (LZW 9-12 bits with RLE
* coding) and Squashing (LZW 9-13).  The basic compression algorithm
* is FC-SWAP (See Storer, James A., _Data Compression Method and
* Theory_, 1989, Computer Science Press).
*
* The great majority of this code came from SQUASH.C by Leslie
* Satensten, which was based on the Unix COMPRESS program which is
* in the Public Domain.
*
* This code has been released into the Public Domain.
*******************************************************************/

int compress(FILE *,FILE *);
int decompress(FILE *, FILE *);

/* the next two codes should not be changed lightly, as they must not */
/* lie within the contiguous general code space. */

#define FIRST 257            /* first free entry */
#define CLEAR 256            /* table clear output code */

/* The tab_suffix table needs 2**BITS characters.  We                     */
/* get this from the beginning of htab.  The output stack uses the rest   */
/* of htab, and contains characters.  There is plenty of room for any     */
/* possible stack (stack used to be 8000 characters).                     */

#define MAXCODE(n_bits)      (( 1<<(n_bits)) - 1)

#define tab_prefixof(i)      codetab[i]
#define tab_suffixof(i)      ((unsigned char *)(htab))[i]
#define de_stack             ((unsigned char *)&tab_suffixof(1<<BITS))

#define BITS       13        /* could be restricted to 12 */
#define INIT_BITS  9         /* initial number of bits/code */

#define HSIZE      9001      /* 91% occupancy */

long htab [HSIZE];
unsigned short codetab [HSIZE];
short hsize ;                     /* for dynamic table sizing */
short clear_flg = 0;
short n_bits;                     /* number of bits/code */
short max_bits = BITS;            /* user settable max # bits/code */
short maxcode;                    /* maximum code, given n_bits */
short maxmaxcode = 1 << BITS;     /* should NEVER generate this code */
int free_ent = 0;                 /* first unused entry */

