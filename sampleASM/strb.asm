; strb.asm
; Program to store a word value and copy its first byte to another word
; Result: WORD1 = 1073, first byte of WORD2 = first byte of 1073

; Data section - directives first
WORD1 .WORD 0

; Code section - instructions second
; Get the address of WORD1 and store it in R2
ADR R2 WORD1

; Load immediate value 1073 into R1
MVI R1 1073

; Store the 1st byte value in R1 into the address of R2
STR R1 [R2]
