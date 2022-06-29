/*******************************************************************
* UNCMP - ERRORS, Version 1.20, created 3-17-90
*
* Common error messages are here to save space.
* (NOTE: as of v1.20 the LZEXE program would probably do a good job
* of crunching the messages, but this is still here for lack of want
* to remove it)
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

void read_error(void)
{
    printf("Error reading file\n");
    exit(1);
}

void write_error(void)
{
    printf("Error writing file\n");
    exit(1);
}

void mem_error(void)
{
    printf("Error allocating memory\n");
    exit(1);
}
