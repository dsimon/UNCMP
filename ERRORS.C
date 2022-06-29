/*******************************************************************
* UNCMP - ERRORS, Version 1.03, created 6-28-89
*
* Common error messages are here to save space.
*
* This code has been released into the Public Domain.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "archead.h"
#include "global.h"
#include "uncmp.h"

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
