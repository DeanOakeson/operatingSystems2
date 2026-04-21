; This is an I/O bound program, it will loop forever doing an SWI instruction

OUTPUT  .BYTE '"'
        .BYTE 's'
        .BYTE 'm'
        .BYTE 'I'
        .BYTE 'o'
        .BYTE '\'
        .BYTE '"'

MVI R2 50
MVI R3 1
LOOP CMP R1 R2
BEQ EXIT
ADR R0 OUTPUT
SWI 1 ; Do some kind of I/O operation here
ADD R1 R1 R3
B LOOP ; Loop to Beginning (infinite loop)
EXIT SWI 10
