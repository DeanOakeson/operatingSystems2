; sub.asm
; Simple program to subtract two values
; Result: R1 = 5 - 256 = -251

; Load immediate value 5 into R2
MVI R2 5

; Load immediate value 256 into R3
MVI R3 256

; Subtract R3 from R2, store result in R1
SUB R1 R2 R3
