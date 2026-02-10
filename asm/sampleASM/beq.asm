; beq.asm
; Program to demonstrate conditional branching with BEQ
; BEQ branches if Z register is equal to zero

; Load immediate value 100 into R1
MVI R1 100

; Load immediate value 100 into R2
MVI R2 100

; Compare R1 and R2, result goes to Z register
; Z = R1 - R2 = 100 - 100 = 0 (equal to zero)
CMP R1 R2

; Branch to EQUAL if Z == 0 (which it is, Z = 0)
BEQ EQUAL

; This instruction will be skipped because Z == 0
MVI R3 999

; Branch target - execution continues here
EQUAL MVI R4 200
