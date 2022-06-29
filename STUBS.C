/*******************************************************************
* UNCMP - STUBS, Version 1.04, created 7-03-89
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
* MSC 5.1.  I think a bug in MSC causes problems with setting the
* DOS errorcode with _nullcheck(), so this one returns an error-
* level of 0 no matter what the main code sets on exit.
*
* This code has been released into the Public Domain (what a joke!).
*******************************************************************/

void _setenvp(void)
{
}

int _nullcheck(void)
{
    return 0;
}
