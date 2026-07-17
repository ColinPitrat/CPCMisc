// Declare C-visible prototypes globally
extern void init();
extern void unlock_asic();
extern void lock_asic();
extern void write_asic_register(unsigned int addr, unsigned char val) __z88dk_callee;
extern unsigned char read_asic_register(unsigned int addr) __z88dk_fastcall;

// Pure ASM definitions (Fully global, clean)
#asm

SECTION code_user
    jp _init

_unlock_seq:
    defb 0xFF, 0x00, 0xFF, 0x77, 0xB3, 0x51, 0xA8, 0xD4, 0x62, 0x39, 0x9C, 0x46, 0x2B, 0x15, 0x8A, 0xCD, 0xEE

// Standard register values: R0 to R17
_crtc_table:
    defb 63, 40, 46, 0x8E, 38, 0, 25, 30, 0, 7, 0, 0, 0x20, 0, 0, 0, 0, 0

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
#endasm

void main()
{
  unsigned char color = 0;
  for (unsigned int addr = 0x8000; addr < 0xC000; addr++) {
    (*(unsigned char*)(addr)) = color;
    color += 1;
  }
  unlock_asic();
  unsigned char start_color = 0;
  while (1) {
    color = start_color;
    for (unsigned int addr = 0x6400; addr < 0x6420; addr++) {
      write_asic_register(addr, color); 
      color += 1;
    }
    start_color += 1;
    for (unsigned int d = 0; d < 1000; d++) {
    }
  }
}
