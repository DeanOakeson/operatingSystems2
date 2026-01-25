; div.asm
; Simple program to divide two values
; Result: R1 = 1024 / 2 = 512

; Load immediate value 1024 into R2
MVI R2 1024

; Load immediate value 2 into R3
MVI R3 2

; Divide R2 by R3, store result in R1
DIV R1 R2 R3
