; bgt.asm
; Program to demonstrate conditional branching with BGT
; BGT branches if Z register is greater than zero

; Load immediate value 100 into R1
MVI R1 100

; Load immediate value 50 into R2
MVI R2 50

; Compare R1 and R2, result goes to Z register
; Z = R1 - R2 = 100 - 50 = 50 (greater than zero)
CMP R1 R2

; Branch to GREATER if Z > 0 (which it is, Z = 50)
BGT GREATER

; This instruction will be skipped because Z > 0
MVI R3 999

; Branch target - execution continues here
GREATER MVI R4 200
