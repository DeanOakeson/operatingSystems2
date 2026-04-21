; bouncing.asm
; does some cpu work to get demoted, then does io to get promoted back

        MVI R1 0
        MVI R2 5; inner loop count
        MVI R3 1
        MVI R5 20; outer loop count

; --- CPU BURST: tight loop to burn quantum and get demoted to queue 1 ---
CPUWORK MVI R4 0
CLOOP   ADD R4 R4 R3
        CMP R4 R2
        BEQ IOLOOP
        B CLOOP

; --- IO BURST: print to trigger SWI, scheduler should promote back ---
IOLOOP  ADR R0 MSG
        SWI 1           ; triggers ISR = promotion

        ADD R1 R1 R3
        CMP R1 R5
        BEQ EXIT
        B CPUWORK

EXIT    SWI 10; halt

MSG     .BYTE '"'
        .BYTE 'b'
        .BYTE 'o'
        .BYTE 'u'
        .BYTE 'n'
        .BYTE 'c'
        .BYTE 'e'
        .BYTE '\'
        .BYTE '"'


