;PRODUCER PROGRAM
;OSHM 5 com.osx <--> prod.osx
;INSM 1 com.osx <--> prod.osx

PBUF .WORD 0
SWI 4
MOV R1 R0           ; R1 = base
MVI R2 4            ; R2 = stride
MVI R3 1            ; R3 = constant 1
MVI R4 0            ; R4 = count
MVI R5 500          ; R5 = limit

;CRITICAL
LOOP SWI 5          ; acquire
LDR R0 [R1]         ; load flag
CMP R0 R3           ; flag == 1?
BEQ PFULL
ADD R4 R4 R3        ; count++
ADD R0 R1 R2        ; R0 = base + 4
STR R4 [R0]         ; store value
STR R3 [R1]         ; set flag = 1
CMP R4 R5           ; count == limit?
BEQ PDONE           ; if done go write sentinel
SWI 6               ; release
B LOOP

PDONE SWI 6               ; release so consumer reads last value
MVI R4 0
ADD R0 R1 R2        ; R0 = base + 4
STR R4 [R0]         ; write sentinel 0 using R4 which is 0
STR R3 [R1]         ; set flag = 1
SWI 10

PFULL SWI 6
SWI 7
B LOOP
