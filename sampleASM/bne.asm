; bne.asm
; Program to demonstrate conditional branching with BNE
; BNE branches if Z register is not equal to zero

; Load immediate value 100 into R1
MVI R1 100

; Load immediate value 50 into R2
MVI R2 50

; Compare R1 and R2, result goes to Z register
; Z = R1 - R2 = 100 - 50 = 50 (not zero)
CMP R1 R2

; Branch to NOTEQUAL if Z != 0 (which it is, Z = 50)
BNE NOTEQUAL

; This instruction will be skipped because Z != 0
MVI R3 999

; Branch target - execution continues here
NOTEQUAL MVI R4 200
