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

;To reduce the number of points of failure, the test loops are unrolled.

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
boot:
  di
  im 1
  ld SP, $dff0
  jp main

; --- Strings ---
s_hello:
.ASC "Bit shifting tests"
.DB $00

s_rlca:
.ASC "RLCA"
.DB $00

s_rla:
.ASC "RLA"
.DB $00

s_bye:
.ASC "End"
.DB $00

s_ok:
.ASC "OK! "
.DB $00

s_err:
.ASC "ERR "
.DB $00

; --- Test cases ---
t_rlca_a:
.DB $1E, $C3, $87, $0F, $1E, $3C, $78, $F0
t_rlca_c:
.DB $01, $01, $01, $01, $00, $00, $00, $00

msg_ok:
  push AF
  push HL
  ld HL, s_ok
  call print_s
  pop HL
  pop AF
  ret
  
msg_err:
  push AF
  push HL
  ld HL, s_err
  call print_s
  pop HL
  pop AF
  ret

test_rlca:
  ;Prepare RLCA test
  ld HL, s_rlca
  call print_s
  call print_nl
  ld HL, t_rlca_a
  ld BC, t_rlca_c
  ld A, $F0
 
  ;Shift 1
  rlca
  call c, msg_ok
  call nc, msg_err  
  ;Shift 2
  rlca
  call c, msg_ok
  call nc, msg_err
  ;Shift 3
  rlca
  call c, msg_ok
  call nc, msg_err
  ;Shift 4
  rlca
  call c, msg_ok
  call nc, msg_err
  
  ;Shift 5
  rlca
  call nc, msg_ok
  call c, msg_err
  ;Shift 6
  rlca
  call nc, msg_ok
  call c, msg_err
  ;Shift 7
  rlca
  call nc, msg_ok
  call c, msg_err
  ;Shift 8
  rlca
  call nc, msg_ok
  call c, msg_err
  ;RLCA test endm
  call print_nl
  ret
  
main:  
  ;Say hello!
  ld HL, s_hello
  call print_s
  call print_nl
  call print_nl
  
  ;RLCA
  call test_rlca
  
  ;Say bye
  call print_nl
  ld HL, s_bye
  call print_s
  
-:
  jr -  
  
.include "./inc/sdsc_print.inc"
