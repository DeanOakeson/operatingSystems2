; CMP.asm
; Program to compare two values using CMP
; Result: Z = R1 - R2 = 100 - 50 = 50

; Load immediate value 100 into R1
MVI R1 100

; Load immediate value 50 into R2
MVI R2 50

; Compare R1 and R2, result stored in Z register
; Z will contain R1 - R2
CMP R1 R2
