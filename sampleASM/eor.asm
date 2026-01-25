; eor.asm
; Program to perform exclusive OR on two values
; Result: R1 = R2 ^ R3 = 255 ^ 170 = 85

; Load immediate value 255 into R2
MVI R2 255

; Load immediate value 170 into R3
MVI R3 170

; Perform EOR operation on R2 and R3, result stored in R1 register
EOR R1 R2 R3
