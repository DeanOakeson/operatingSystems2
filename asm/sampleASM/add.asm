; add.asm
; Simple program to add two values
; Result: R1 = 5 + 256 = 261

; Load immediate value 5 into R2
MVI R2 5

; Load immediate value 256 into R3
MVI R3 256

; Add R2 and R3, store result in R1
ADD R1 R2 R3
