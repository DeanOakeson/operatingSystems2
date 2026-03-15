PSTR    .BYTE '"' 
        .BYTE '\'
        .BYTE 'P'
        .BYTE 'A'
        .BYTE 'R'
        .BYTE 'E'
        .BYTE 'N'
        .BYTE 'T'
        .BYTE '\'
        .BYTE '\'
        .BYTE '"'

CSTR    .BYTE '"'
        .BYTE '\'
        .BYTE 'C'
        .BYTE 'H'
        .BYTE 'I'
        .BYTE 'L'
        .BYTE 'D'
        .BYTE '\'
        .BYTE '\'
        .BYTE '"'

GSTR    .BYTE '"'
        .BYTE '\'
        .BYTE 'G'
        .BYTE 'R'
        .BYTE 'A'
        .BYTE 'N'
        .BYTE 'D'
        .BYTE '$'
        .BYTE 'C'
        .BYTE 'H'
        .BYTE 'I'
        .BYTE 'L'
        .BYTE 'D'
        .BYTE '\'
        .BYTE '\'
        .BYTE '"'



SWI 3 ;FORK
BNE CPR ;BRANCH TO CPR IF Z = 1 .. set in interupt service handler

;PARENT PRINT PROGRAM
ADR R0 PSTR
SWI 1 ;PRINT PSTR
B HALT

;CHILD PRINT PROGRAM
CPR ADR R0 CSTR
CMP R1 R2; CHANGE CHILD Z BACK TO 0;
SWI 3
BNE GPR
SWI 1 ;PRINT CSTR
B HALT

;GRANDCHILD PRINT PROGRAM
GPR ADR R0 GSTR
SWI 1
B HALT

HALT SWI 10 ; HALT
