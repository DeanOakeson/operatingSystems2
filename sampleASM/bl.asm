; bl.asm
; Program to demonstrate branch and link with BL
; BL saves the return address (PC+6) in R5 and jumps to a subroutine

; Main program
; Load immediate value 10 into R1
MVI R1 10

; Branch and link to SUBROUTINE label
; This saves the return address in R5
BL SUBROUTINE

; Execution continues here after returning from subroutine
; Load value 30 into R3
MVI R3 30

; End of main program
B END

; Subroutine
SUBROUTINE MVI R2 20

; Return from subroutine by branching to address in R5
BX R5

; End label
END MVI R4 40
