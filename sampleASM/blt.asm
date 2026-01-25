; blt.asm
; Program to demonstrate conditional branching with BLT
; BLT branches if Z register is less than zero

; Load immediate value 50 into R1
MVI R1 50

; Load immediate value 100 into R2
MVI R2 100

; Compare R1 and R2, result goes to Z register
; Z = R1 - R2 = 50 - 100 = -50 (less than zero)
CMP R1 R2

; Branch to LESS if Z < 0 (which it is, Z = -50)
BLT LESS

; This instruction will be skipped because Z < 0
MVI R3 999

; Branch target - execution continues here
LESS MVI R4 200
