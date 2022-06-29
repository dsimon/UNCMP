/*******************************************************************
* UNCMP - HUFFMAN, Version 1.03, created 6-28-89
*
* Classic Huffman with RLE uncompression module.
*
* The great majority of this code came from SQUPRT33 by Theo Pozzy
* which borrowed code from SQ and USQ, and USQ by Richard Greenlaw.
* Both of the above mentioned programs are in the Public Domain.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "archead.h"
#include "global.h"
#include "uncmp.h"

#define SQEOF      256                 /* Squeeze EOF */
#define NUMVALS    257                 /* 256 data values plus SQEOF */

struct sq_tree                         /* decoding tree */
{
    int children[2];                   /* left, right */
    } dnode[NUMVALS];                  /* use large buffer */

void sq_decomp(FILE *in, FILE *out)    /* initialize Huffman unsqueezing */
{
    register int i;                    /* generic loop index */
    register int bitpos;               /* last bit position read */
    int curbyte;                       /* last byte value read */
    int numnodes;

    /* get number of nodes in tree, this uses two character input calls */
    /* instead of one integer input call for speed */

    numnodes = getc_pak(in) | (getc_pak(in)<<8);

    if ((numnodes < 0) || (numnodes >= NUMVALS)) {
         printf("File has invlaid decode tree\n");
         exit(1);
         }

    /* initialize for possible empty tree (SQEOF only) */

    dnode[0].children[0] = -(SQEOF + 1);
    dnode[0].children[1] = -(SQEOF + 1);

    for(i=0; i<numnodes; ++i) {        /* get decoding tree from file */
         dnode[i].children[0] = getc_pak(in) | (getc_pak(in)<<8);
         dnode[i].children[1] = getc_pak(in) | (getc_pak(in)<<8);
         }

    bitpos = 8;                        /* set to above read condition */

    while (i != EOF) {
         for(i = 0; i >= 0; ) {                 /* traverse tree               */
              if(++bitpos > 7) {
                   if((curbyte=getc_pak(in)) == EOF)
                        return;
                   bitpos = 0;

                   /* move a level deeper in tree */

                   i = dnode[i].children[1 & curbyte];
                   }
              else i = dnode[i].children[1 & (curbyte >>= 1)];
              }

         /* decode fake node index to original data value */

         i = -(i + 1);

         /* decode special endfile token to normal EOF */

         i = (i == SQEOF) ? EOF : i;
         if (i != EOF) putc_rle(i,out);
         }
    }\Rogue\Monster\
else
  echo "will not over write ./huffman.c"
fi
if `test ! -s ./link.lst`
then
echo "writing ./link.lst"
cat > ./link.lst << '\Rogue\Monster\'
UNCMP+
STUBS+
DISPATCH+
CRC+
FILEIO+
GLOBAL+
DLZW1213+
TESTARC+
GETHEAD+
LISTARC+
FILELIST+
HUFFMAN+
STORE+
ERRORS+
PACK+
SLZW12
UNCMP.EXE
UNCMP
/NOE /NOI /E /M;\Rogue\Monster\
else
  echo "will not over write ./link.lst"
fi
if `test ! -s ./listarc.c`
then
echo "writing ./listarc.c"
cat > ./listarc.c << '\Rogue\Monster\'
/*******************************************************************
* UNCMP - LISTARC, Version 1.03, created 6-28-89
*
* Archive directory lister.
*
* Lists contents of archives in standard verbose style.
*
* The great majority of this code came from AV v2.01 by Derron
* Simon, of which the source code has not been released, however it
* this code is in the Public Domain.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <string.h>
#include "archead.h"
#include "global.h"
#include "uncmp.h"

void list_arc(FILE *in)
{
    unsigned long int total_size = 0L;
    unsigned long total_length = 0L;
    unsigned int total_files = 0;
    char method[9];
    char crushing = 0;
    int hour, min, sec;
    int month, day, year;

    printf("  Filename      Length   Method   SF     Size   Date     Time    CRC\n"
           "  --------      ------   ------   --     ----   ----     ----    ---\n");

    do {
         switch ((int) archead.atype) {
              case 1:
                   strcpy(method, " stored ");
              case 2:
                   strcpy(method, " Stored ");
                   break;
              case 3:
                   strcpy(method, " Packed ");
                   break;
              case 4:
                   strcpy(method, "Squeezed");
                   break;
              case 5:
              case 6:
              case 7:
                   strcpy(method, "crunched");
                   break;
              case 8:
                   strcpy(method, "Crunched");
                   break;
              case 9:
                   strcpy(method, "Squashed");
                   break;
              case 10:
                   strcpy(method, "Crushed*");
                   crushing++;
                   break;
              default:
                   strcpy(method, "Unknown!");
         }

         year = (archead.date >> 9) & 0x7f;      /* dissect the date */
         month = (archead.date >> 5) & 0x0f;
         day = archead.date & 0x1f;

         hour = (archead.time >> 11) & 0x1f;     /* dissect the time */
         min = (archead.time >> 5) & 0x3f;
         sec = (archead.time & 0x1f) * 2;

         if (check_list(archead.name)) {
              printf("  %-12s %7lu  %8s  %02d%% %7lu %2d-%02d-%2d %02d:%02d:%02d %04X\n", archead.name, archead.length, method,
                   calcsf(archead.length, archead.size), archead.size, month, day, year + 80, hour, min, sec, archead.crc);
              total_length += archead.length;
              total_size += archead.size;
              total_files++;
              }
         fseek(in,archead.size,1);
    } while (getarcheader(in) != 0);
    printf("  --------      ------            --     ----\n");
    printf("  Total: %-3d   %7lu            %02d%% %7lu\n", total_files, total_length, calcsf(total_length, total_size), total_size);
    if (crushing > 0) printf("\n* Crushing not supported in this version of UNCMP\n");
    fclose(in);
}

int calcsf(long length_now, long org_size)
{
    if (length_now == 0) return 0; /* avoid divide-by-zero error */

    /* divide in fixed point, to avoid FP */

    return ((int) (100 - ((org_size * 100) / (length_now))));
}
