#include <cpc.h>
#include <stdio.h>
#include <stdlib.h>

int i = 0;
char interrupt_index = 0;

// To work, this requires exporting _fw_int_address__ in
// tools/z88dk/libsrc/target/cpc/classic/cpc_crt0.asm
// cf. https://www.z88dk.org/forum/viewtopic.php?t=11705
extern void *_fw_int_address__;
void *original_int_address;

void change_color()
{
  #asm
  ld bc,0x7F00
  // GateArray register 0 to 0 (pen selection)
  out (c), c
  // GateArray register 1 to a (color selection)
  ld a, 0x43
  out (c), a
  #endasm
}

void interrupt_callback()
{
  #asm
  di
  push af
  push bc
  ld a, (_interrupt_index)
  inc a
  cp 6
  jp nz, not_last_interrupt_in_frame
  xor a

.not_last_interrupt_in_frame
  ld (_interrupt_index), a

/*
  ld bc,0x7F00
  // GateArray register to 0 to 0 (pen selection)
  out (c),c
  // GateArray register to 1 to a (color selection)
  or 0x40
  out (c),a
  */

  pop bc
  pop af

  push hl
  // Regular interrupt handler
  ld hl, original_interrupt_callback_return
  push hl
  ld hl, (_original_int_address)
  push hl
  ret

.original_interrupt_callback_return
  pop hl

  ei
  #endasm
}

void setup_interrupt()
{
  #asm
  di
  // Save original interrupt interposer
  ld hl, (__fw_int_address__)
  ld (_original_int_address), hl
  // Add our own interposer
  ld hl, _interrupt_callback
  ld (__fw_int_address__), hl

/* This is what we would do without z88dk's interposer
  // Save original interrupt callback
  ld hl, (0x39)
  ld (_original_interrupt_callback), hl

  // Replace it with our own interrupt callback
  ld hl, _interrupt_callback
  ld (0x39), hl
*/
  ei
  #endasm
}

void main()
{
  //change_color();
  //fgetc_cons();

  setup_interrupt();

  for (int i = 0; i < 20; i++)
  {
    printf("Hello, world!: %d\n", interrupt_index);
  }

  fgetc_cons();
}
