; ldrb.asm
; Program to store a value to memory and load only the first byte using LDRB
; Result: R3 = first byte of 2034

; Data section - directives first
VALUE .WORD 0

; Code section - instructions second
; Get the address of VALUE and store it in R2
ADR R2 VALUE

; Load immediate value 2034 into R1
MVI R1 2034

; Store the value in R1 to VALUE (using address in R2)
STR R1 [R2]

; Load only the first byte from memory[R2] into R3
LDRB R3 [R2]
