/*******************************************************************
* UNCMP - LICENSE, Version 1.20, created 3-17-90
*
* This is the central dispatch routine used by DISPATCH, OUTPUT and
* TESTARC to uncompress files.
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
#include "uncmp.h"
#include "archead.h"
#include "global.h"

void douncmp(FILE *in, FILE *out)
{
    crc = 0;
    sizeleft = archead.size;
    state = 0;

    switch (archead.atype) {
         case 1:
         case 2:
              store_decomp(in, out);
              break;
         case 3:
              rle_decomp(in, out);
              break;
         case 4:
              sq_decomp(in, out);
              break;
         case 5:
         case 6:
         case 7:
              slzw_decomp(in, out, archead.atype);
              break;
         case 8:
              dlzw_decomp(in, out, CRUNCH);
              break;
         case 9:
              dlzw_decomp(in, out, SQUASH);
              break;
         default:
              break;
              /* should never reach this code */
         }
    }