;-------------------------------------------------------------------------
; UNCMP - GETCODE3.ASM, Version 1.20, created 3-17-90
;
; 80386 version of bit input for DLZW1213.
;
; Inputs the correct number of bits for decompress() in module
; DLZW1213 and does basic housekeeping.  This function has been
; re-written from the MSC generated assembly code.  It does improve
; speed a little bit, and provides no more functionality.
;
; The C version of this code came from SQUASH.C by Leslie Satensten,
; which was based on the Unix COMPRESS.
;
; All of UNCMP is Copyright (C) 1989-90 Derron Simon
;
; This program is free for everyone, but I retain all copyrights over
; it.  Please see USER.MAN for more information.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
;-------------------------------------------------------------------------

.MODEL SMALL,C                 ; set for small model C interface
.386                           ; using 80386 instructions

EXTRN  __acrtused:ABS
EXTRN  _filbuf:NEAR
EXTRN  sizeleft:DWORD
EXTRN  free_ent:WORD
EXTRN  max_bits:WORD
EXTRN  maxcode:WORD
EXTRN  maxmaxcode:WORD
EXTRN  clear_flg:WORD
EXTRN  n_bits:WORD

.DATA

$S349_offset   DW      00H
$S350_size     DW      00H
$S354_rmask    DB      00H
       DB      01H
       DB      03H
       DB      07H
       DB      0fH
       DB      01fH
       DB      03fH
       DB      07fH
       DB      0ffH
;_BSS      SEGMENT
$S347_iobuf    DB 0dH DUP (?)
       EVEN
;_BSS      ENDS

.CODE
       PUBLIC  getcode
getcode        PROC NEAR
       push    bp
       mov     bp,sp
       sub     sp,10
       push    di
       push    si
;      in = 4
;      register si = code
;      register di = r_off
;      bits = -4
;      bp = -2

       mov     WORD PTR [bp-2],OFFSET DGROUP:$S347_iobuf
                                       ; move the address of iobuf into bp

       mov     ax,$S350_size           ; if (offset >= size) goto $I356
       cmp     $S349_offset,ax
       jge     $I356
       cmp     WORD PTR clear_flg,0    ; if (clear_flg > 0) goto $I356
       jg      $I356
       mov     ax,maxcode              ; if (free_ent <= maxcode) goto $I355
       cmp     free_ent,ax
       jle     $I355
$I356:
       mov     di,$S350_size           ; temporarily use di for size
       mov     ax,maxcode              ; if (free_ent <= maxcode) goto $I357
       cmp     free_ent,ax             ; else continue
       jle     $I357
       inc     WORD PTR n_bits         ; n_bits++
       mov     ax,max_bits             ; if (n_bits != max_bits) goto $I358
       cmp     n_bits,ax
       jne     $I358
       mov     ax,maxmaxcode           ; else move maxmaxcode into ax
       jmp     SHORT $L20004           ; continue past next code
$I358:
       mov     ax,1
       mov     cl,BYTE PTR n_bits      ; ((1 << n_bits)) - 1)
       shl     ax,cl
       dec     ax
$L20004:
       mov     maxcode,ax              ; replace maxcode with ax
$I357:
       cmp     WORD PTR clear_flg,0    ; if (clear_flg <= 0) goto $I360
       jle     $I360
       mov     WORD PTR maxcode,511    ; maxcode = 1 << 9 - 1
       mov     WORD PTR n_bits,9       ; nbits = 9
       mov     WORD PTR clear_flg,0    ; reset clear_flg
$I360:
       sub     di,di                   ; r_off = 0
       jmp     SHORT $L20003           ; go to loop
$I364:
       sub     sizeleft,1              ; sizeleft--
       mov     bx,WORD PTR [bp+4]      ; move in to bx
       dec     WORD PTR [bx+2]         ; (in->cnt)--
       js      $L20000                 ; if (in->cnt < 0) goto $L20000
       mov     ax,WORD PTR [bx]        ; inline getc()
       inc     WORD PTR [bx]
       mov     bx,ax
       movzx   ax,BYTE PTR [bx]
       jmp     SHORT $L20001
$L20000:
       push    WORD PTR [bp+4]         ; push in
       call    _filbuf                 ; call buffer filling routine
       add     sp,2                    ; pop in
$L20001:
       mov     si,ax                   ; iobuf[r_off] = ax
       mov     BYTE PTR $S347_iobuf[di],al
       inc     di
$L20003:
       cmp     n_bits,di               ; if (n_bits <= r_off) goto $FB363
       jle     $FB363
       mov     eax,sizeleft            ; move sizeleft to eax
       or      eax,eax                 ; if (sizeleft) goto $I364 ???
       jne     $I364
$FB363:
       or      di,di                   ; if (r_off <= 0) continue
       jg      $I365                   ; else go on
       mov     ax,-1                   ; set return of -1
       pop     si                      ; clean up
       pop     di
       mov     sp,bp
       pop     bp
       ret                             ; return
$I365:
       mov     WORD PTR $S349_offset,0 ; set offset = 0
       shl     di,3                    ; r_off = r_off << 3 - n_bits - 1
       sub     di,n_bits
       inc     di
       mov     $S350_size,di           ; size = r_off
$I355:
       mov     di,$S349_offset         ; r_off = offset
       mov     ax,n_bits               ; ax = n_bits
       mov     WORD PTR [bp-4],ax      ; bits = n_bits
       mov     ax,di                   ; bp = (r_off >> 3)
       sar     ax,3
       add     WORD PTR [bp-2],ax      ; set bp
       and     di,7                    ; r_off &= 7
       mov     bx,WORD PTR [bp-2]      ; set bx = bp
       inc     WORD PTR [bp-2]         ; bp++
       movzx   ax,BYTE PTR [bx]        ; set ax = *bp
       mov     cx,di                   ; set cx = r_off
       shr     ax,cl                   ; *bp >> r_off
       mov     si,ax                   ; code = ax
       mov     ax,8                    ; ax = 8
       sub     ax,di                   ; ax = ax - r_off
       mov     WORD PTR [bp-10],ax
       sub     WORD PTR [bp-4],ax      ; bits -= 8 - r_off
       mov     di,ax                   ; reset r_off
       cmp     WORD PTR [bp-4],8       ;
       jl      $I366
       inc     bx                      ; bx++ (bx contains bp)
       inc     WORD PTR [bp-2]         ; bp++
       movzx   ax,BYTE PTR [bx]        ; al = *bp
       mov     cx,di                   ; cx = r_off
       shl     ax,cl                   ;
       or      si,ax
       add     di,8
       sub     WORD PTR [bp-4],8       ; bits = bits - 8
$I366:
       mov     bx,WORD PTR [bp-4]      ; set bx = bits
       movzx   ax,BYTE PTR $S354_rmask[bx]
                                       ; set ax = rmask[bits]
       mov     bx,WORD PTR [bp-2]      ; set bx = bp
       movzx   cx,BYTE PTR [bx]        ; set cl = *bp
       and     ax,cx                   ; rmask[bits] & *bp
       mov     cx,di                   ; cx = r_off
       shl     ax,cl                   ; ax << r_off
       or      si,ax                   ; code |= ax
       mov     ax,n_bits               ; ax = n_bits
       add     $S349_offset,ax         ; offset += n_bits
       mov     ax,si                   ; ax = code
       pop     si                      ; return code
       pop     di
       mov     sp,bp
       pop     bp
       ret

getcode        ENDP
               END
