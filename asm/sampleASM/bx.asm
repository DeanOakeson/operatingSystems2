; bx.asm
; Program to demonstrate branching to an address stored in a register with BX
; BX branches to the address contained in the specified register

; Load immediate value 5 into R1
MVI R1 5

; Get the address of TARGET label and store it in R2
ADR R2 TARGET

; Branch to the address stored in R2
BX R2

; This instruction will be skipped
MVI R3 999

; Branch target - execution continues here
TARGET MVI R4 100
