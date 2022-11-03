org #6000
SCR_SET_MODE  EQU #BC0E
TXT_OUTPUT    EQU #BB5A
TXT_SET_COL   EQU #BB6F
TXT_SET_ROW   EQU #BB72
KM_WAIT_CHAR  EQU #BB06

main:
call fill_screen_2
;call test_print_num
;call test_display_hl
ret

print_space:
  ld a, ' '
  call TXT_OUTPUT
  ret

test_display_hl:
  ld h, 0
  ld l, 0
  call DispHL
  call print_space
  CALL KM_WAIT_CHAR

  ld h, 0
  ld l, 13
  call DispHL
  call print_space
  CALL KM_WAIT_CHAR

  ld h, 0
  ld l, 159
  call DispHL
  call print_space
  CALL KM_WAIT_CHAR

  ; 7830
  ld h, 30
  ld l, 150
  call DispHL
  call print_space
  CALL KM_WAIT_CHAR

  ld h, 0xFF
  ld l, 0xFF
  call DispHL
  call print_space
  CALL KM_WAIT_CHAR
 
  ret

test_print_num:
  ld a, 0
  push af
  call print_num
  CALL KM_WAIT_CHAR

  ld a, 13
  push af
  call print_num
  CALL KM_WAIT_CHAR

  ld a, 27
  push af
  call print_num
  CALL KM_WAIT_CHAR

  ld a, 159
  push af
  call print_num
  CALL KM_WAIT_CHAR

  ld a, 254
  push af
  call print_num
  CALL KM_WAIT_CHAR

  ld a, 255
  push af
  call print_num
  CALL KM_WAIT_CHAR

  RET

fill_screen_1:

    LD A, 0
    CALL SCR_SET_MODE

    LD A, 0
    LD HL, 0xC000
  Fill:
    LD (HL), A
    INC L
    JP NZ,Fill
    INC A
    INC H
    JP NZ,Fill

    CALL KM_WAIT_CHAR

  RET

fill_screen_2:
  ld a, 1
  call SCR_SET_MODE

  ; Loop counter (and "color"), from 0 to 255
  ld  d, 0

  startloop:
    ld  e,d
    push  de

  ;memset
    ld  hl,#C000
    ; Set the first byte to e
    ld  (hl), e
    ld  e, l
    ld  d, h
    inc de
    ; Now (de) point to the byte just after (hl)
    ld  bc, #3FFF
    ; Copy from (hl) to (de), inc them, decrement bc and repeat until bc = 0
    ldir

    pop de
    ;push de
    ; Move cursor to (0,0)
    ld a, 1
    call TXT_SET_COL
    ld a, 1
    call TXT_SET_ROW
    ; Output current color (print_num only prints d)
    ld a, e
    call print_num

    CALL KM_WAIT_CHAR

    ld  a, d
    sub #FF
    jr  NC,endloop
    inc d
    jr  startloop
  endloop:

  CALL KM_WAIT_CHAR

  RET

; Print the content of a as a decimal number
print_num:
  ld bc, 2            ; Allocate 2 bytes on the stack
  call allocate_stack_space
  sub 10
  jp C,print_digit    ; a < 10, print it directly
  ld b, 0
div_10:               ; find quotient and remainder for 10
  inc b
  sub 10
  jp NC,div_10
  ld (hl), a          ; Save the remainder
  ld a, b
  call print_num      ; Print the quotient first
  ld hl, 0
  add hl, sp
  ld bc, 4            ; Account for the space taken by free_stack_space method address and size to free
  adc hl, bc
  ld a, (hl)          ; Find back where the remainder is
print_digit:
  add a, 10
  add a, 0x30
  call TXT_OUTPUT
  ret

; Allocate bc bytes on the stack, automatically freed on next ret
; hl will point to the data
allocate_stack_space:
  pop ix              ; Return address
  ld hl, 0
  add hl, sp
  sbc hl, bc
  ld sp, hl
  push bc             ; How much to free
  ld bc, free_stack_space
  push bc             ; Method to free (next ret after this function will call this)
  push ix
  ret

free_stack_space:
  pop hl              ; Size to free
  add hl, sp
  ld sp, hl           ; Restore stack pointer
  ret                 ; Will return to the caller's caller

DispHL:
	ld	bc,-10000
	call	Num1
	ld	bc,-1000
	call	Num1
	ld	bc,-100
	call	Num1
	ld	c,-10
	call	Num1
	ld	c,-1
Num1:	ld	a,'0'-1
Num2:	inc	a
	add	hl,bc
	jr	c,Num2
	sbc	hl,bc
	call TXT_OUTPUT
	ret 
