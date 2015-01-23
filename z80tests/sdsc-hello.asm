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

;Write 'HELLO!' to the SDSC debug console
        org 00h
        LD A, 'H'
        OUT (0fdh), A
        LD A, 'A'
        OUT (0fdh), A
        LD A, 'L'
        OUT (0fdh), A
        OUT (0fdh), A
        LD A, 'O'
        OUT (0fdh), A
        LD A, '!'
        OUT (0fdh), A
        LD A, '\n'
        OUT (0fdh), A
;Clear screen
		LD A, 002h
		OUT (0fch), A
;Write HELLO! again
        LD A, 'H'
        OUT (0fdh), A
        LD A, 'E'
        OUT (0fdh), A
        LD A, 'L'
        OUT (0fdh), A
        OUT (0fdh), A
        LD A, 'O'
        OUT (0fdh), A
        LD A, '!'
        OUT (0fdh), A
        LD A, '\n'
        OUT (0fdh), A

loop:
        NOP
        JP loop
