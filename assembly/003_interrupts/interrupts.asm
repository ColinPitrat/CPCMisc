nolist
org &4000

  call install_interrupts

  ; Ensure low ROM is disabled
  ld bc, &7fbd
  out (c), c

  ; Infinite loop
  jp $

  ret

install_interrupts:
  di
  ld hl, interrupt_callback
  ld ( &39 ), hl
  ei
  ret

interrupt_notReady equ -2
interrupt_firstValue equ -1

interrupt_callback:

  ld ( interrupt_previous_stack ), sp
  ld sp, interrupt_stack_start 

  push af
  push bc
  push de
  push hl
  push ix
  push iy
  exx
  ex af, af'
  push af
  push bc
  push de
  push hl
	push ix
	push iy

  ld b, &f5
  in a, ( c )
  rrca
  jr nc, skipInitFirst
  ld a, interrupt_firstValue
  ld ( interrupt_index ), a

skipInitFirst:

  ld a, ( interrupt_index )
  cp interrupt_notReady
  jp z, skipInterrupt

  inc a
  cp 6
  jp nz, no_interrupt_reset
  xor a

no_interrupt_reset:
  ld ( interrupt_index ), a

  add a, a
  ld c, a
  ld b, 0
  ld hl, interrupts
  add hl, bc

  ld a, ( hl )
  inc hl
  ld h, ( hl )
  ld l, a
  ld ( interrupt_call + 1 ), hl

interrupt_call:
  call 0

skipInterrupt:

	pop iy
	pop ix
  pop hl
  pop de
  pop bc
  pop af
  ex af, af'
  exx
  pop iy
  pop ix
  pop hl
  pop de
  pop bc
  pop af

  ld sp, ( interrupt_previous_stack )

  ei
  ret

interrupt_previous_stack:
  dw 0 
interrupt_stack:
  ds 256
interrupt_stack_start:

interrupt_index:
  db interrupt_notReady
interrupts:
  dw interrupt_0
  dw interrupt_1
  dw interrupt_2
  dw interrupt_3
  dw interrupt_4
  dw interrupt_5

COLOR0 equ &54
COLOR1 equ &44
COLOR2 equ &55
COLOR3 equ &5C
COLOR4 equ &58
COLOR5 equ &5D
COLOR6 equ &4C

setColor:
  ld bc, &7f00
  out (c), c
  out (c), a
  ld bc, &7f10
  out (c), c
  out (c), a
  ret

interrupt_0:
  ld a, COLOR0
  jp setColor

interrupt_1:
  ld a, COLOR1
  jp setColor

interrupt_2:
  ld a, COLOR2
  jp setColor

interrupt_3:
  ld a, COLOR3
  jp setColor

interrupt_4:
  ld a, COLOR4
  jp setColor

interrupt_5:
  ld a, COLOR5
  jp setColor
