org #6000
TXT_OUTPUT    EQU #BB5A
KM_WAIT_CHAR    EQU #BB06

  ld hl, message
loop:
  ld a,(hl)
  cp 0
  jr Z,exit
  call TXT_OUTPUT
  inc hl
  jp loop

exit:
  call KM_WAIT_CHAR

ret

message:
  defm "Hello world"
  defb 0
