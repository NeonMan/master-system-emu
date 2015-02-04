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

; Prints a string to SDSC console
;     (HL) -- Null-terminated string
print_s:
  push AF
  push HL
  -:
    ld A, (HL)
    cp $00
    jp Z, +
    out ($FD), A
    inc HL
    jr -
  +:  
  pop HL
  pop AF
  ret
  
; Prints byte stored in A reg to SDSC
;     A -- Byte to print
print_a:
  push AF
  push BC
  push DE
  push HL
  
  ld B, A      ;
  ld C, A      ; Copy the original A value
  
  ;Upper nibble (B)
  SRL B
  SRL B
  SRL B
  SRL B
  ;Lower Nibble (C)
  LD A, C
  AND $0F
  LD C, A
  
  ;Print upper nibble (B)
  LD HL, hex_lut ;
  LD DE, $0000   ;
  LD E, B        ;
  ADD HL, DE     ; Point to hex_lut + nibble value
  LD A, (HL)
  OUT ($FD), A   ; Print to SDSC
  ;Print lower nibble (C)
  LD HL, hex_lut ;
  LD DE, $0000   ;
  LD E, C        ;
  ADD HL, DE     ; Point to hex_lut + nibble value
  LD A, (HL)
  OUT ($FD), A   ; Print to SDSC
  
  pop HL
  pop DE
  pop BC
  pop AF
  ret

hex_lut:
.ASC "0123456789ABCDEF"