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

;--------------------------------------------------------------------------
;  Based off the SDCC z80 Crt0. 
;
;  Modifications made so it can relocate into RAM and run from there.
;--------------------------------------------------------------------------

	.module crt0
	.globl	_main
	.area	_HEADER (ABS)
	
	.org	0x0000
	.db 0x00
	.ascii "Self-reloacating ROM. Trim all data before the 0xc000 offset"
	
	;; Reset vector
	.org 	0xc000
entry:
	;;No interrupts. we cannot use most of those anyway...
	di
	jr init
	ENTRY_DIFFERENCE = ret_check - entry
	RELOCATE_SIZE = relocate_end - relocate
	RELOCATE_ADDRESS = 0xDFF0 - RELOCATE_SIZE
	RELOCATE_OFFSET  = relocate - entry
	RAM_BASE_ADDRESS = 0xc000
	
init:
	;; Set stack pointer directly above top of memory minus one byte
	ld	sp,#0xDFFF
	
	;; Set SEGA mapper to the default 0,1,2 slots
	ld  hl,#0xFFFD
	ld  (hl),#0x00
	inc hl
	ld  (hl),#0x01
	inc hl
	ld  (hl),#0x02
	
	;;write a 'RET' instruction (0xC9) at the 0xDFFF address
	ld hl,#0xDFFF
	ld (hl),#0xC9
	
	;;Call 0xDFFF, this way we have the _real_ address of ret_check
	;;On the stack
	call #0xDFFF
ret_check:
	;;Make SP point to the 'return' address, real address of ret_check
	dec sp
	dec sp
	;;The real address of 'ret_check' is now ready to be 'popped out'.
	;;We must eventually have the real address of 'entry' ready to be
	;;used for an LDIR copy-relocation of the program. (HL reg)
	
	;;Calculate the difference between 'ret_check' and 'entry'
	;;And push it back into the stack
	pop hl
	ld bc,#ENTRY_DIFFERENCE
	scf
	ccf
	sbc hl,bc
	push hl
	
	;;The real address of 'entry' is now at the stack top, now we need to
	;;copy the whole program from there to the RAM base address (0xC000)
	;;But first... the opcodes required to do this must be outside of the
	;;copy path, so we write it just below the stack.
	ld de,#RELOCATE_ADDRESS
	  ;;Calculate real address of relocate
	  pop hl
	  dec sp ;;
	  dec sp ;;Make SP point to the real entry address again
	  scf
	  ccf
	  ld bc,#RELOCATE_OFFSET
	  adc hl,bc
	ld bc,#RELOCATE_SIZE
	ldir
	;;Jump to relocation. We will never get back to the original ROM copy
	jp RELOCATE_ADDRESS

relocate:
	;;Objective, copy 4K of data from the real address of 'entry' to the 
	;;base address of RAM (0xC000)
	ld de,#RAM_BASE_ADDRESS
	pop hl ;;<-- Retrieve the real address of 'entry'
	ld bc,#0x1000
	ldir
	;;Move SP back to the ram top
	ld sp,#0xE000
	;;By now all data should be on its correct place. So we call _main
	call	_main
	jp	_exit
relocate_end:
	
	
	
	
	
	;;Signature to easily check the Crt being used.
crt_signature:
	.ascii "SMS RAM Crt0 V0.1"
	.db 0x00
	
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
	halt	 ; NONI/HALT used for emulator hooks.
	jr	1$

