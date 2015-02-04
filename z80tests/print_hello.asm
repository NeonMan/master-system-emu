; Copyright 2015 Juan Luis �lvarez Mart�nez
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
  ; Test print_s
	ld HL, s_hello
	call print_s
  
  ; Test print_a
  ld HL, s_8bit
  call print_s
  ld A, $42
  call print_a

loop:
        NOP
        JP loop

s_hello:
.ASC "HELLO WORLD!!"
.DB $0A, $00

s_8bit:
.ASC "Print A: 0x"
.DB $00


.include "sdsc_print.inc"
