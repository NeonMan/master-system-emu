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
; -------------------
; --- Entry point ---
; -------------------
boot:
  di
  im 1
  ld SP, $dff0
  jp main

; --- Includes ---
.include "./inc/sdsc_print.inc"
  
; --- Strings ---
s_hello:
.ASC "Bit shifting tests"
.DB $00

s_rlca:
.ASC "RLCA"
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
t_rlca:
.DB $1E, $01
.DB $C3, $01
.DB $87, $01
.DB $0F, $01
.DB $1E, $00
.DB $3C, $00
.DB $78, $00
.DB $F0, $00
t_rlca_end:

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
  ld HL, s_rlca
  call print_s
  call print_nl
  
  ;Prepare RLCA test
  ;D contains the iteration counter
  ld D, $08
  ;Initial A value
  ld A, $F0

  ; ---------------------------
  ; --- Run the instruction ---
  ; ---------------------------
  inst_loop:
    ;Perform the rotation
    rlca
    ;Store the results in the stack
    push AF
    ;Decrement iteration counter
    dec D
    jp NZ, inst_loop
    
  ; ------------------------------------
  ; --- Compare against known values ---
  ; ------------------------------------
  ;use A as iteration counter
  ld A, $08
  ;HL points to the test cases
  ld HL, t_rlca_end - 1
  check_loop:
    ;Move counter to shadow register
    ex AF, AF'
    pop BC     ;Get calculated AF result
    ld D, (HL) ;Get stored F
    dec HL
    ld E, (HL) ;Get stored A
    dec HL
    
    ld A, C          ;
    cp E             ;
    call NZ, msg_err ;
    call Z,  msg_ok  ;Check 'A'
    
    ld A, B          ;
    and $01
    cp C             ;
    call NZ, msg_err ;
    call Z,  msg_ok  ;Check 'F'
    
    ;Retrieve iteration counter form shadow reg
    ex AF, AF'
    dec A  ;Decrement counter
    jp NZ, check_loop
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
