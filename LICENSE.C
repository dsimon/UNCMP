/*******************************************************************
* UNCMP - LICENSE, Version 1.20, created 3-17-90
*
* Displays distribution license and warranty to stdout.
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
#include "uncmp.h"
#include "archead.h"
#include "global.h"

void license(void)
{
    printf("This program is free softare; you can redistribute it freely as long as\n");
    printf("you do not charge any more than reproduction fees.\n\n");
    printf("This program is distributed in the hope that it will be useful,\n");
    printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    printf("programs documentation for more details.\n\n");
    printf("All of UNCMP is copyright (c) 1989-1990 by Derron Simon\n");
    printf("Derron Simon/14 Valley Road/Glen Mills, PA 19342/(215)459-2981\n");
}
