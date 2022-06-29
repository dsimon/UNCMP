/*******************************************************************
* UNCMP - STUBS, Version 1.20, created 3-17-90
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
* MSC 5.1.  If _nullcheck() doesn't return a value, the error code set
* by exit() is lost, regardless of the value set by _nullcheck.
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

void _setenvp(void)
{
}

int _nullcheck(void)
{
    return 0;
}
