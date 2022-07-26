# UNCMP
 
Released by Derron Simon in 1989 and updated through early 1990.  ARC was one of the first lossless data compression and archival formats for PCs.  UNCMP was my fast unarchiver.  It followed AV, my archive viewer, and was itself followed by CMP, my archiver.  Unfortunately the source code to AV is lost to time (the filename was AV20SRC.ZIP or AV20SRC.ZOO; please reach out if you have a copy!).  The source code to CMP suffered the same fate.

See https://en.wikipedia.org/wiki/ARC_(file_format) for more information on the ARC file format.

Below you will find the content of PROGRAMR.MAN, which was my prehistoric version of a README.md, formatted for Markdown.  (Although it surprising took very little adjusting!)  The original can be found within the repository.  Enjoy!

--

UNCMP v1.20 PROGRAMMER MANUAL
-----------------------------

About this file
---------------
This document is intended to be helpful for programmers who are interested in
the workings of UNCMP.  People who are not interested in hacking UNCMP should
skip this file and read the USER.MAN file included in this archive.

Development system used
-----------------------
The author wrote UNCMP on a GRiDCASE 1530 80386 based portable running MS-DOS
3.30 with 1 megabyte of memory.  QC v2.0 was used for development and testing,
while the final version was compiled with MSC 5.1.  Turbo C v2.00 was tested
with UNCMP occasionally and with the final version.  I used the EC text editor
from C Source.

MSC & QC dependant information
-------------------------
The STUBS.C file has two null functions that speed things up a little with
MSC and Quick C.  The SMALL model is used.

TC dependant information
------------------------
Original versions of UNCMP (1.03 and 1.04) used the HUGE memory model with
Turbo C.  This was because UNCMP declared too much static memory.  This has
been changed and now the Turbo C version of UNCMP is closer to the MSC version
in speed and size.  If you use Turbo C, be sure to pack the executable file
with EXEPACK (if you own it).

MAKE files included
-------------------
The source for UNCMP contains makefiles for use with MSC, Quick C, and Turbo C.
The Quick C makefile uses Microsoft's NMAKE syntax, the MSC makefile uses
Microsoft's MAKE syntax and the Turbo C makefile uses Borland's MAKE syntax.  A
generic MS-DOS C makefile for use with NDMAKE45 is also included.

Quick C & Turbo C integrated environments
-----------------------------------------
The makefile for use in the Microsoft Quick C integrated environment is called
UNCMP.MAK.  The files UNCMP.PRJ and UNCMP.TC are the project file and
configuration for Turbo C.

Development history
-------------------
UNCMP was originally created to extract all crunched (arctype 8) and squashed
(arctype 9) files from an archive.  It later added the ability to extract every
archive type except crushing (arctype 10) and the ability to extract only named
files.

General background on arc files
-------------------------------
The ARC format of archives has become the industry standard since System
Enhancement Associates created the original ARC archive utility.  Since then it
has added and removed many different compression systems in an attempt to create
the smallest archives.  Only type 2,3 and 8 are used by the current version of
ARC and only type 2,3,8 and 9 are used by PKARC.  The other types are obsolete,
but still supported by UNCMP.  Type 10 is used by PAK, a product of NoGate
Consultants, but UNCMP doesn't support it since not all the specs are available.

     #   Description
    ---  ----------------------------------------------------------------------
     1   no compression, but archive header is shorter than current version.
     2   no compression, with new header.
     3   RLE compression.
     4   Squeezing (or classic Huffman) with RLE
     5   Lempel-Ziv static 12 bit without RLE
     6   Lempel-Ziv static 12 bit with RLE
     7   Lempel-Ziv static 12 bit with RLE and new hash method.
     8   Dynamic Lempel-Ziv-Welch 9-12 bit with RLE.
     9   Dynamic Lempel-Ziv-Welch 9-13 bit without RLE.
    10   Dynamic Lempel-Ziv-Welch 2-13 bit without RLE and deletion of
         non-frequently used nodes.  Used by files with the extension PAK.
    11   Sliding-Windows Huffman.  This is all I know about this format.

Archive header
--------------
The format for the archive header can be found in ARCHEAD.H.  The general
format of an archive is as follows:

<ARCHIVE_HEADER>
    COMPRESSED_FILE
<ARCHIVE_HEADER>
    COMPRESSED_FILE
<ARCHIVE_END>

<ARCHIVE_HEADER> is the header for each individual file in the archive.
<ARCHIVE_END> is a two-byte pair (0x1a,0x00).

Wish list of enhancements
-------------------------
What follows is a wish list of enhancements that I'd be more than happy to see
implemented if you've got the time.

  - Support for garbled files (encrypted)
  - Rewrite getcode() in module DLZW1213.C in assembly
    (NOTE: as of version 1.04, getcode is right now near optimal speed)
    (NOTE: as of version 1.20, getcode is in assembly, but not hand
     coded.  A handcoded 80386 version would be nice ;>)
  - *Faster RLE decoding (with large buffer handling)*
  - Support for crushing (arctype 10)
  - Support for wildcard archive names (ie. 'UNCMP -o *.arc *.c' would extract
    all the *.C files from every ARC file in the current directory)
  - Self-extracting archive capability
    (How do you make an EXE think of the rest of itself as an archive?)
  - *Compatiblity with more archive formats (ZIP/LZH)?  What do you think?*

Any speed improvements to UNCMP are welcome.

Problems in portability
-----------------------
I'm not sure of where problems will be in porting, however I believe that the
only real system dependant stuff is in UNCMP.C and deals with overwrite
checking and filestamp setting.

ANSI C compiler portability
---------------------------
Compiling UNCMP with a generic ANSI compatible compiler should prove to be no
problem.  Just define the MSC extensions FAR, NEAR and PASCAL to nothing and
define the far-memory allocations routines to standard malloc() and free().
If you are compiling UNCMP on an MS-DOS system, you will need to use the
COMPACT memory model (64k code, 1mb data).

Errors
------
UNCMP compiled with MSC and Quick C with full warnings will warn of many
data conversions.  Turbo C will warn of possible loss of precision.  Both
of these errors should be ignored (maybe I'll fix 'em someday, but not now).

Commenting
----------
I believe that most of the code is well-commented and should be easy to read.

Testing
-------
If you have a version up and running for your compiler, please send me the
sources and information at one of the BBS's mentioned in the USER.MAN file.

Optimizations
-------------
Most of the optimizations I performed were adding the 'register' keyword
to frequently used variables and speeding things up with inline functions.  I
removed the function putc_pak() and changed it to inline output and addcrc.
I found that although MSC claims that declaring variables as 'register' may
inhibit MSC's ability to optimize, a human makes better choices in which
variable is accessed most frequently.  This has made UNCMP very comparable in
speed with ARC v6.0x.

Program size considerations
---------------------------
Starting with v1.20 I have begun using LZEXE v0.90 instead of EXEPACK.  LZEXE
is a new utility from France that will compress EXE files using LZW (crunching)
and extract the program to memory every time it is run.  On my 80386 system
this adds a 1/2 second overhead and reduces the size of the executable by 33%,
which I consider well worth it.  LZEXE can probably be obtained from the same
sources as UNCMP, but it is not required to generate an UNCMP with your C
compiler.  Just remove the line:
  LZEXE UNCMP.EXE
from the make file and make it normally.


Where my priorities are in writing UNCMP
----------------------------------------
UNCMP was originally going to be released without source code, so I didn't care
much for portability and readability.  Later, I decided to clean up the source
and release UNCMP with full source code.  Since then I've been faced with the
question of whether speed or portability are more important.  I know that in a
perfect world a program could be fast and portable, but in the PC world it
has become apparent that they are mutually exclusive.  I have chosen speed as
the most important of the two.  To achieve greater speed I have reduced the
readability of UNCMP by replacing the input/output functions with inline
functions.  All successive versions of UNCMP will hopefully be faster, but if
you would rather have portability and readability, than I suggest the v1.03
(first release) sources, since it was very readable and will probably be the
most portable of all the versions to come.

PAK Support
-----------
I'd love to have working source code for uncrushing and undistilling.  If you
have a working version, please send it to me!  Even if you just have an
algorithm, I'll implement it!  NoGate has not even released specs on
distilling, it only calls it sliding-windows huffman.  I assume this is a
sliding window of about 4096 bytes (only a guess) and the compressor searches
for the first appearence of a byte (or sub-string) and sends the position as
a huffman code (therefor the most recently inputted character is closest and
uses the shortest code).  This is purely a guess.  In implementing the
uncrushing algorithm, how is the least-recently-used node determined and how
frequently is the table purged?  If you have any of the answers, please contact
me.

Update information
------------------
- 1.03
  - First release.
- 1.04
  - Second release.
  - Various speed optimizations.
    - removed putc_pak() and changed to inline.
    - added NEAR defines.
    - moved getcode() to a separate file.
- 1.10
  - Third release.
  - Many code rewrites.
    - Changed static table declarations on SLZW, HUFFMAN, and DLZW1213 to dynamically allocated far memory.
    - Because of the above, UNCMP can now be compiled in the small model.
  - Added copyright information.
  - Added extract to console support.
- 1.12
  - Fourth modification (not released).
  - Removed GNU licensing.
  - Added extract newer files option.
- 1.20
  - **Now 33% smaller because of the use of LZEXE in development (see above).**
  - Rewrote getcode() in assembly language for MSC and QC.
  - **Added 386 support in the file GETCODE3.ASM.  When make-d (?) with MAKE386.MSC it will be included.**
  - Added compiler level 286 support.
  - Rewrote parts of the main code to move important defines to header files.
