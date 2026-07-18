// Declare C-visible prototypes globally
extern void init();
extern void unlock_asic();
extern void lock_asic();
extern void write_asic_register(unsigned int addr, unsigned char val) __z88dk_callee;
extern unsigned char read_asic_register(unsigned int addr) __z88dk_fastcall;
extern void draw_char(unsigned char *font_data, unsigned char *screen_addr) __z88dk_callee;
extern unsigned char read_key_row(unsigned int row) __z88dk_callee;
extern void read_keyboard_matrix();
extern void delay_ms(unsigned int ms) __z88dk_fastcall;

extern char keyboard_matrix[10];


#define BOOL unsigned char
#define TRUE 1
#define FALSE 0

// Pure ASM definitions (Fully global, clean)
#asm

SECTION code_user
ORG 0x0000
    jp _init

_unlock_seq:
    defb 0xFF, 0x00, 0xFF, 0x77, 0xB3, 0x51, 0xA8, 0xD4, 0x62, 0x39, 0x9C, 0x46, 0x2B, 0x15, 0x8A, 0xCD, 0xEE

// Standard register values: R0 to R17
_crtc_table:
    defb 63, 40, 46, 0x8E, 38, 0, 25, 30, 0, 7, 0, 0, 0x30, 0, 0, 0, 0, 0

_pens_table:
    defb 0x44, 0x4A, 0x53, 0x4C, 0x4B, 0x54, 0x55, 0x4D, 0x46, 0x5E, 0x5F, 0x47, 0x52, 0x59, 0x54, 0x54

// Set the RST 0x38 interrupt handler (just ret)
// We have:
//  - 2 bytes for jp _init
//  - 17 bytes of unlock sequence
//  - 18 bytes of CRTC registers
//  - 16 bytes for pens
// This leaves 3 bytes to fill before reaching 0x38 = 56.
_fill: 
    defs 3
    ret


PUBLIC _init
_init:
  di
  // Set stack to a reasonable place
  ld sp, 0xC000
  // Initialize CRTC
  ld hl, _crtc_table
  ld b, 18
  ld c, 0
_crtc_loop:
  push bc
  // Select register
  ld b, 0xbc
  out (c), c
  // Write data
  ld a, (hl)
  ld bc, 0xbd00
  out (c), a
  // Restore counter
  pop bc
  inc hl
  inc c
  djnz _crtc_loop
  // Set mode 1
  ld bc, 0x7f00
  ld a, 0x89
  out (c), a
  // Init pens
  ld hl, _pens_table
  ld b, 16
  ld c, 0
_pal_loop:
  push bc
  ld b, 0x7f
  out (c), c
  ld a, (hl)
  out (c), a
  pop bc
  inc hl
  inc c
  djnz _pal_loop
  // Set border color
  ld bc, 0x7f00
  ld a, 0x10
  out (c), a
  // Color 11 (4b) = white
  // Color 4  (44) = blue
  ld a, 0x44
  out (c), a
  ei
  jp _main

PUBLIC _unlock_asic
_unlock_asic:
    di
    ld bc, 0xBC00
    ld hl, _unlock_seq
    ld e, 17
_unlock_loop:
    ld a, (hl)
    out (c), a
    inc hl
    dec e
    jr nz, _unlock_loop
    ei
    ret

PUBLIC _lock_asic
_lock_asic:
    di
    ld bc, 0xBC00
    ld a, 0x42
    out (c), a
    ei
    ret

PUBLIC _write_asic_register
_write_asic_register:
    di
    // Map ASIC registers to &4000-&7FFF
    ld bc, 0x7FB8
    out (c), c
    
    pop hl              // HL = return address
    pop bc              // C = value
    pop de              // DE = address
    push hl             // Put return address back
    
    ld a, c
    ld (de), a

    // Unmap ASIC registers
    ld bc, 0x7FA0
    out (c), c

    ei
    ret

PUBLIC _read_asic_register
_read_asic_register:
    di
    // Map ASIC registers to &4000-&7FFF
    ld bc, 0x7FB8
    out (c), c
    
    // Address is in HL via __z88dk_fastcall linkage
    ld a, (hl)
    ld l, a             // Return value in L
    ld h, 0
    // Unmap ASIC registers
    ld bc, 0x7FA0
    out (c), c
    ei
    ret

PUBLIC _draw_char
// Routine: draw_char
// Uses:  BC, A, HL, DE
_draw_char:
    pop bc             // Return address
    pop de             // Screen Address (screen_addr)
    pop hl             // Font Data Address (font_data)
    push bc            // Restore Return address to top of stack
    ld b, 8            // 8 lines to draw

_row_loop:
    push bc            // Save line counter

    // Convert 1-bit font to 2-bit Mode 1 pixel data
    // A 1-bit '1' becomes binary '11' (color 3)
    // TODO: support other colors.
    ld a, (hl)
    and 0xF0           // Look at the first 4 pixels
    ld b, a
    rrc
    rrc
    rrc 
    rrc
    or b
    ld (de), a
    inc de             // Next byte

    ld a, (hl)         // Get next font byte (remaining 4 pixels)
    and 0x0F
    ld b, a
    rlc 
    rlc 
    rlc
    rlc
    or b
    ld (de), a
    dec de             // Restore X for the next line

    // Move to next scanline
    // A standard CPC line jump is +&800
    ld a, d
    adc a, 0x08        // Add 08 to high byte (0x800 total)
    ld d, a

    pop bc
    inc hl
    djnz _row_loop
    ret

PUBLIC _read_keyboard_matrix
// Shamelessly copied from https://cpctech.cpcwiki.de/source/keyboard.html
_read_keyboard_matrix:
  ld hl,_keyboard_matrix

  ld bc,0xf40e               // write PSG register index (14) to PPI port A (databus to PSG)
  out (c),c

  ld b,0xf6
  in a,(c)
  and 0x30
  ld c,a

  or 0xC0                    // bit 7=bit 6=1 (PSG operation: write register index)
  out (c),a                  // set PSG operation -> select PSG register 14

  // at this point PSG will have register 14 selected.
  // any read/write operation to the PSG will act on this register.

  out (c),c                  // bit 7=bit 6=0 (PSG operation: inactive)

  inc b
  ld a,0x92
  out (c),a                  // write PPI control: port A: input, port B: input, port C upper: output
                             // port C lower: output
  push bc
  set 6,c                    // bit 7=0, bit 6=1 (PSG operation: read register data)

_scan_key:
  ld b,0xf6
  out (c),c                 // set matrix line & set PSG operation

  ld b,0xf4                 // PPI port A (databus to/from PSG)
  in a,(c)                  // get matrix line data from PSG register 14

  cpl                       // invert data: 1->0, 0->1
                            // if a key/joystick button is pressed bit will be "1"
                            // keys that are not pressed will be "0"

  ld (hl),a                 // write line data to buffer
  inc hl                    // update position in buffer
  inc c                     // update line

  ld a,c
  and 0x0f
  cp 0x0a                   // scanned all rows?
  jr nz,_scan_key           // no loop and get next row

  // scanned all rows
  pop bc

  ld a,0x82                 // write PPI Control: Port A: Output, Port B: Input, Port C upper: output, Port C lower: output.
  out (c),a

  dec b
  out (c),c                 // set PSG operation: bit7=0, bit 6=0 (PSG operation: inactive)
  ret

PUBLIC _delay_ms
// Wait for HL milliseconds
_delay_ms:
  push hl
  pop bc
_delay_ms_outer:
  ld de, 0x008D             // Determined experimentally to match 1ms
_delay_ms_inner:
  dec de
  ld a, d
  or e
  jr nz, _delay_ms_inner
  dec bc
  ld a, b
  or c
  jr nz, _delay_ms_outer
  ret

#endasm

#include "charmap.h"

void put_char(unsigned int x, unsigned int y, char c) {
  if (c < 0x20) {
    return;
  }
  c -= 0x20;
  unsigned char *addr = 0xC000 + 80*y + 2*x;
  draw_char(charmap[c], addr);
}

void print(unsigned int x, unsigned int y, char* message) {
  char c;
  while ((c = *message++) != 0) {
    if (x >= 40) {
      x = 0;
      y += 1;
    }
    // TODO: Not the best behavior: returning to the top when reaching the bottom.
    if (y >= 25) {
      y = 0;
    }
    put_char(x, y, c);
    x += 1;
  }
}

void wait_for_keypressed() {
  while (TRUE) {
    read_keyboard_matrix();
    for (unsigned int i = 0; i < 10; i++) {
      // Very weird bug in zcc: if keyboard_matrix is used directly in the `if`,
      // then the label generated will be in bss_user instead of being in the code!
      char k = keyboard_matrix[i];
      // If the value is not 0, it means at least one key in this row is pressed
      if (k != 0) {
        return;
      }
    }
  }
  return;
}

void main()
{
  unsigned char color = 0;
  unlock_asic();
  for (unsigned int addr = 0x6400; addr < 0x6420; addr++) {
    write_asic_register(addr, color); 
    color += 1;
  }

  print(0, 0, "/");
  print(39, 0, "\\");
  print(0, 24, "\\");
  print(39, 24, "/");
  print(8, 1, "Caprice32 test cartridge");

  print(3, 15, "Press any key to start timer");
  wait_for_keypressed();
  print(3, 15, "Timer started               ");
  delay_ms(1000);
  print(3, 15, "Press any key to continue...");
  wait_for_keypressed();

  for (unsigned int addr = 0xC000; addr < 0xFFFF; addr++) {
    (*(unsigned char*)(addr)) = color;
    color += 1;
  }

  while (TRUE) {
  }
}

#asm
// We have to define all these compiler specific section to ensure they are not moved to 0x8000.
// Not sure why!
SECTION rodata_compiler
SECTION bss_compiler
SECTION code_math
SECTION code_l
SECTION code_l_sccz80

SECTION bss_user
ORG 0x8000
PUBLIC _keyboard_matrix
// This buffer has one byte per keyboard line.
// Each byte defines a single keyboard line, which defines
// the state for up to 8 keys.
// A bit in a byte will be '1' if the corresponding key
// is pressed, '0' if the key is not pressed.
_keyboard_matrix: defs 10

#endasm
