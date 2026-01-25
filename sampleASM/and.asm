; and.asm
; Program to perform bitwise AND on two values
; Result: R1 = R2 & R3 = 255 & 15 = 15

; Load immediate value 255 into R2
MVI R2 255

; Load immediate value 15 into R3
MVI R3 15

; Perform AND operation on R2 and R3, result stored in R1 register
AND R1 R2 R3
