

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
