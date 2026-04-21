;CONSUMER PROGRAM
;OSHM 5 com.osx <--> prod.osx
;INSM 1 com.osx <--> prod.osx
NEWLINE .BYTE '$'

CBUF .WORD 0
SWI 4
MOV R1 R0           ; R1 = base
MVI R2 4            ; R2 = stride
MVI R3 1            ; R3 = constant 1
MVI R4 0            ; R4 = read count
MVI R5 500          ; R5 = limit

;CRITICAL
LOOP SWI 5               ; acquire
LDR R0 [R1]         ; load flag
CMP R0 R3           ; flag == 1?
BNE CEMPTY
ADD R0 R1 R2        ; R0 = base + 4
LDR R0 [R0]         ; load value
CMP R0 R4           ; value == 0? sentinel check

BEQ CDONE
SWI 0               ; print int
MVI R0 0
STR R0 [R1]         ; clear flag = 0
ADD R4 R4 R3        ; read count++
SWI 6               ; release
ADR R0 NEWLINE
SWI 1               ;print newline
CMP R4 R5
BEQ CDONE
BLT LOOP

CDONE SWI 6
SWI 10

CEMPTY SWI 6
B LOOP


