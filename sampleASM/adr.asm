; adr.asm
; Program to get the address of a labeled data value
; Result: R2 = address of VAL

; Data section - directives first
VAL .WORD 42

; Code section - instructions second
; Get the address of VAL and store it in R2
ADR R2 VAL
