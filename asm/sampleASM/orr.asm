; orr.asm
; Program to perform bitwise OR on two values
; Result: R1 = R2 | R3 = 240 | 15 = 255

; Load immediate value 240 into R2
MVI R2 240

; Load immediate value 15 into R3
MVI R3 15

; Perform ORR operation on R2 and R3, result stored in R1 register
ORR R1 R2 R3	
