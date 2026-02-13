; b.asm
; Program to demonstrate unconditional branching with B
; This will jump over the instruction that loads 999 into R2
RUN .WORD 50

; Load immediate value 100 into R1
MVI R1 100
MVI R3 999

; Unconditionally branch to SKIP label
B SKIP

; This instruction will be skipped and never executed
MVI R2 999

; Branch target - execution continues here
SKIP MVI R4 200
