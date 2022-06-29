PROJ   =UNCMP
DEBUG  =1
CC     =qcl
CFLAGS_G       = /AS /W3 /Ze /DQC
CFLAGS_D       = /Zi /Zr /Gi$(PROJ).mdt /Od
CFLAGS_R       = /O /Ol /DNDEBUG
CFLAGS =$(CFLAGS_G) $(CFLAGS_D)
LFLAGS_G       = /CP:0xffff /NOI /NOE /SE:0x80 /ST:0x800
LFLAGS_D       = /CO /INCR
LFLAGS_R       =
LFLAGS =$(LFLAGS_G) $(LFLAGS_D)
RUNFLAGS       =-t c:\dl\cdocum.arc
OBJS_EXT =
LIBS_EXT =

all:   $(PROJ).exe

uncmp.obj:     uncmp.c

crc.obj:       crc.c

dispatch.obj:  dispatch.c

dlzw1213.obj:  dlzw1213.c

douncmp.obj:   douncmp.c

errors.obj:    errors.c

filelist.obj:  filelist.c

getcode.obj:   getcode.asm

gethead.obj:   gethead.c

global.obj:    global.c

huffman.obj:   huffman.c

license.obj:   license.c

listarc.obj:   listarc.c

output.obj:    output.c

pack.obj:      pack.c

rle.obj:       rle.c

slzw12.obj:    slzw12.c

store.obj:     store.c

stubs.obj:     stubs.c

testarc.obj:   testarc.c

$(PROJ).exe:   uncmp.obj crc.obj dispatch.obj dlzw1213.obj douncmp.obj errors.obj filelist.obj \
       getcode.obj gethead.obj global.obj huffman.obj license.obj listarc.obj output.obj \
       pack.obj rle.obj slzw12.obj store.obj stubs.obj testarc.obj $(OBJS_EXT)
       echo >NUL @<<$(PROJ).crf
uncmp.obj +
crc.obj +
dispatch.obj +
dlzw1213.obj +
douncmp.obj +
errors.obj +
filelist.obj +
getcode.obj +
gethead.obj +
global.obj +
huffman.obj +
license.obj +
listarc.obj +
output.obj +
pack.obj +
rle.obj +
slzw12.obj +
store.obj +
stubs.obj +
testarc.obj +
$(OBJS_EXT)
$(PROJ).exe

$(LIBS_EXT);
<<
       ilink -a -e "link $(LFLAGS) @$(PROJ).crf" $(PROJ)

run: $(PROJ).exe
       $(PROJ) $(RUNFLAGS)

