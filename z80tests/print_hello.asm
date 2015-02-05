; Copyright 2015 Juan Luis Álvarez Martínez
; 
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
; 
; http ://www.apache.org/licenses/LICENSE-2.0
; 
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

; WLA-DX banking setup
.memorymap
defaultslot 0
slotsize $8000
slot 0 $0000
.endme

.rombankmap
bankstotal 1
banksize $8000
banks 1
.endro

.bank 0 slot 0
.org $0000
;Write 'HELLO!' to the SDSC debug console
main:
  di
  im 1
  ld SP, $dff0
  
  ; Test print_s
	ld HL, s_hello
	call print_s
  
  ; Test print_a
  ld HL, s_8bit
  call print_s
  ld HL, s_0x
  ld A, $42
  
  ;Print numbers from 0x42 to 0x51
  -:
    cp $52
    jp Z, +
    call print_s
    call print_a
    call print_nl
    inc A
  jp -
  +:
  
  ;Test print_bc
  ld HL, s_printbc
  call print_s
  ld HL, s_0x
  
  ld BC, $CAFE
  call print_s
  call print_bc
  call print_nl
  inc BC
  call print_s
  call print_bc
  call print_nl
  
  ;BYE!
  ld HL, s_bye
  call print_s
  
loop:
        NOP
        JP loop

s_hello:
.ASC "HELLO WORLD!!"
.DB $0A, $00

s_0x:
.ASC "0x"
.DB $00

s_8bit:
.ASC "Print A:"
.DB $0a, $00

s_printbc:
.ASC "Print BC:"
.DB $0A, $00

s_bye:
.ASC "BYE!"
.DB $0a, $00

.include "sdsc_print.inc"
