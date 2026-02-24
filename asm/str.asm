; str.asm
; Program to store a value to memory using register indirect addressing
; Result: memory[address of VAL] = 1072

; Data section - directives first
VAL .WORD 42

; Code section - instructions second
; Get the address of VAL and store it in R2
ADR R2 VAL

; Load immediate value 1072 into R1
MVI R1 1072

; Store the value in R1 to the memory address in R2
STR R1 [R2]
