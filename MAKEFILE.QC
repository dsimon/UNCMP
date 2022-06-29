####################################################################
# UNCMP - MAKEFILE.QC, Version 1.10, created 7-15-89
#
# Microsoft NMAKE makefile for creating UNCMP v1.10.
#
# This files using another file named LINK.LST to give to LINK
# because the number of files is too large for LINK to handle from
# the command line.
#
# This code has been released into the Public Domain.
####################################################################

PROJ     = UNCMP
CC       = qcl
AS       = masm

# the following options choose: optimize for size, use compact model,
# compile only, include symbolic debugging information, use maximum
# warning level and defines MSC.

CFLAGS   = /Ox /AS /Zi /c /W3 /DQC
AFLAGS   = /Mx

.c.obj:
       $(CC) $(CFLAGS) $*.c
.asm.obj:
       $(AS) $(AFLAGS) $*;

.SUFFIXES:.obj.exe.asm.c

all: $(PROJ).exe

crc.obj: crc.c

dispatch.obj: dispatch.c

dlzw1213.obj: dlzw1213.c

errors.obj: errors.c

filelist.obj: filelist.c

getcode.obj: getcode.asm

gethead.obj: gethead.c

global.obj: global.c

huffman.obj: huffman.c

license.obj: license.c

listarc.obj: listarc.c

output.obj: output.c

pack.obj: pack.c

rle.obj: rle.c

slzw12.obj: slzw12.c

store.obj: store.c

stubs.obj: stubs.c

testarc.obj: testarc.c

uncmp.obj: uncmp.c

douncmp.obj: douncmp.c

$(PROJ).exe: crc.obj dispatch.obj dlzw1213.obj errors.obj filelist.obj getcode.obj \
    gethead.obj global.obj huffman.obj license.obj listarc.obj output.obj pack.obj rle.obj \
    slzw12.obj store.obj stubs.obj testarc.obj uncmp.obj douncmp.obj $(OBJS_EXT)
    link @LINK.LST
    LZEXE UNCMP.EXE