FIRST .BYTE '"'
      .BYTE 'd'
      .BYTE 'e'
      .BYTE 'a'
      .BYTE 'n'
      .BYTE '$'
      .BYTE '"'

LAST  .BYTE '"'
      .BYTE 'o'
      .BYTE 'a'
      .BYTE 'k'
      .BYTE 'e'
      .BYTE 's'
      .BYTE 'o'
      .BYTE 'n'
      .BYTE '\'
      .BYTE '"'


;GET ADDRESS OF STRING OR CHAR IN R0
;PRINT CHAR/STRING
ADR R0 FIRST
SWI 1

;GET ADDRESS OF STRING OR CHAR IN R0
;PRINT LAST INITIAL
ADR R0 LAST
SWI 1

HALT SWI 10
