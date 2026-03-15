CHILD   .BYTE 'C'
        .BYTE 'H'
        .BYTE 'I'
        .BYTE 'L'
        .BYTE 'D'
        .BYTE '\'
        .BYTE '~'
PARENT
        .BYTE 'P'
        .BYTE 'A'
        .BYTE 'R'
        .BYTE 'E'
        .BYTE 'N'
        .BYTE 'T'
        .BYTE '\'
        .BYTE '~'

SWI 23 ;FORK

;PARENT
MVI R1 255
MVI R2 900
MVI R3 800

;CHILD
MVI R4 700
MVI R0 998
MVI R5 13
