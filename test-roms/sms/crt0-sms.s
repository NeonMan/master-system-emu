;--------------------------------------------------------------------------
;  crt0.s - crt0.s for the SEGA master system, Z80 based system.
;
;  Copyright (C) 2000, Michael Hope
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

	.module crt0
	.globl	_main

	.area	_HEADER (ABS)
	
	;; SDSC ROM header
	.org 0x7fe0
	.ascii "SDSC"
	.db 0xff,0xff           ; Version (BCD)
	.db 0xff,0xff,0xff,0xff ; Date DD/MM/YYYY (BCD)
	.db 0xff,0xff           ; Author pointer
	.db 0xff,0xff           ; Name pointer
	.db 0xff,0xff           ; Description pointer
	
	;; SEGA ROM header
	.org 0x7FF0
	.ascii "TMR SEGA"     ; SEGA string
	.db 0xff,0xff         ; Reserved
	.db 0xff,0xff         ; Checksum
	.db 0xff,0xff,0xff    ; Product ID and version
	.db 0xff              ; Region and size
	
	;; Reset vector
	.org 	0
	di
	jp	init

	.org	0x08
	call _rst_08h
	reti

	.org	0x10
	call _rst_10h
	reti

	.org	0x18
	call _rst_18h
	reti

	.org	0x20
	call _rst_20h
	reti

	.org	0x28
	call _rst_28h
	reti

	.org	0x30
	call _rst_08h
	reti

	.org	0x38
	call _rst_38h
	reti

nmi_hook:
	;NMI call code does not fit it INT1 is being used, so we make an extra
	;jump before performing the call
	call _nmi
	reti
	
	;NMI vector
	.org	0x66
	jr nmi_hook
	
	;INT vector
	.org	0x68
	call _int1
	reti
	
	.org	0x50
	.ascii "SMS crt0 V0.1  "
	.db 0x00
	
	;.org	0x100
	.org	0x70
init:
	;; Set stack pointer directly above top of memory.
	ld	sp,#0xE000
	;; Set SEGA mapper to the default 0,1,2 slots
	ld  hl,#0xFFFD
	ld  (hl),#0x00
	inc hl
	ld  (hl),#0x01
	inc hl
	ld  (hl),#0x02

	;; Initialise global variables
gsinit::
	ld	bc, #l__INITIALIZER
	ld	a, b
	or	a, c
	jr	Z, gsinit_next
	ld	de, #s__INITIALIZED
	ld	hl, #s__INITIALIZER
	ldir
gsinit_next:
	call	_main
	jp	_exit

	;; Ordering of segments for the linker.
	.area	_HOME
	.area	_CODE
	.area	_INITIALIZER
	.area   _GSINIT
	.area   _GSFINAL

	.area	_DATA
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP

	.area   _CODE

_exit::
	;; Exit - special code to the emulator
	ld	a,#0xFF
1$:
	.db 0xFD ;
	halt     ; NONI/HALT used for emulator hooks.
	jr	1$

