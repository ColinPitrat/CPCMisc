#include <conio.h>
#include <cpc.h>
#include <stdio.h>
#include <stdlib.h>

#include "sound.c"

#define TXT_SET_CURSOR 0xbb75
#define KL_TIME_PLEASE 0xBD0D

#define MIN_Y 8
#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 160
#define HALF_SCREEN_WIDTH 80

#define NB_STARS 80
#define MAX_NB_ENEMIES 20

void cpc_SetTxtCursorPos(int x, int y)
{
  #asm
  EXTERN firmware
  // Stack contains:
  //  SP: return address
  //  SP+2: y
  //  SP+4: x
  ld hl,2
  add hl,sp
  ld e,(hl)
  inc hl
  inc hl
  ld d,(hl)
  ex de,hl
  // H=column L=line
  //ld hl, 0x050d
  call firmware
  defw TXT_SET_CURSOR
  #endasm
}

// x in [0, 640]
// y in [0, 200]
// color = 0b0x0x0x0x (i.e p1 color, no matter whether x is odd or even)
void putpixel_m0(int x, int y, unsigned char color)
{
  // The screen is from 0xC000 to 0xFFFF.
  // 0xC000-0xC780 is the first row of pixels of the 25 lines of characters.
  // 0xC800-0xCF80 is the second row of pixels of the 25 lines of characters.
  // 0xD000-0xD780 is the third row of pixels of the 25 lines of characters.
  // etc...
  // 0xF800-0xFF80 is the eighth row of pixels of the 25 lines of characters.
  //
  // In mode 0, each byte represents the color of 2 pixels:
  // p = pixel, b = bit
  // p0b0, p1b0, p0b2, p1b2, p0b1, p1b1, p0b3, p1b3
  // So 0b00011011 = 0x1B means:
  //  - p0 has color 0b0101 = 5
  //  - p1 has color 0b0011 = 3
  //
  // Address of a pixel at x, y is:
  // 0xC000 + ((y / 8) * 80) + ((y % 8) * 2048) + x / 2
  // (and it's mixed with the pixel at x-1 or x+1)
  unsigned char* address = 0xC000 + ((y / 8) * HALF_SCREEN_WIDTH) + ((y % 8) * 2048) + (x >> 1);
  unsigned char mask = 0xAA;
  if ((x & 1) == 0)
  {
    color = color << 1;
    mask = mask >> 1;
  }
  *address = (*address & mask) | color;
}

#define PLAYER_WIDTH 7
#define PLAYER_HEIGHT 20

#define PLAYER_Y_SPEED 6
#define PLAYER_X_SPEED 2

#define MIN_PLAYER_Y MIN_Y
#define MAX_PLAYER_X (HALF_SCREEN_WIDTH-PLAYER_WIDTH)
#define MAX_PLAYER_Y (SCREEN_HEIGHT-PLAYER_HEIGHT)

#define PLAYER_START_X 0
#define PLAYER_START_Y (MIN_PLAYER_Y+((MAX_PLAYER_Y-MIN_PLAYER_Y)>>1));

unsigned char player_sprite[PLAYER_WIDTH*PLAYER_HEIGHT] = {
  0x3C, 0x3C, 0x3C, 0x3C, 0x2A, 0x00, 0x00,
  0x3C, 0x3C, 0x3C, 0x3C, 0x2A, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x03, 0x03, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x03, 0x03, 0xFC, 0xFC, 0x03, 0x03,
  0x00, 0x03, 0x03, 0xFC, 0xFC, 0x03, 0x03,
  0x00, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00,
  0x00, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x03, 0x03, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x03, 0x03, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,
  0x3C, 0x3C, 0x3C, 0x3C, 0x2A, 0x00, 0x00,
  0x3C, 0x3C, 0x3C, 0x3C, 0x2A, 0x00, 0x00,
};

#define MISSILE_WIDTH 5
#define MISSILE_HEIGHT 3
#define MISSILE_SPEED 5
#define MAX_MISSILE_X HALF_SCREEN_WIDTH-MISSILE_WIDTH

unsigned char missile_sprite[MISSILE_WIDTH*MISSILE_HEIGHT] = {
  0x00, 0xFC, 0x00, 0xFC, 0x00,
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C,
  0x00, 0xFC, 0x00, 0xFC, 0x00
};

#define ENEMY_WIDTH 7
#define ENEMY_HEIGHT 20

#define ENEMY_Y_SPEED 6
#define ENEMY_X_SPEED -2

#define MIN_ENEMY_X 0
#define MAX_ENEMY_X HALF_SCREEN_WIDTH-ENEMY_WIDTH
#define MIN_ENEMY_Y MIN_Y
#define MAX_ENEMY_Y SCREEN_HEIGHT-ENEMY_HEIGHT

// 0,11   -> 01000101 -> 0x45
// 9,9    -> 11000011 -> 0xC3
// 9,10   -> 10000111 -> 0x87
// 9,11   -> 11000111 -> 0xC7
// 10,9   -> 01001011 -> 0x4B
// 10,10  -> 00001111 -> 0x0F
// 10,11  -> 01001111 -> 0x4F
// 11,0   -> 10001010 -> 0x8A
// 11,9   -> 11001011 -> 0xCB
// 11,10  -> 10001111 -> 0x8F
// 11,11  -> 11001111 -> 0xCF
// p0b0, p1b0, p0b2, p1b2, p0b1, p1b1, p0b3, p1b3
unsigned char enemy_sprite[ENEMY_WIDTH*ENEMY_HEIGHT] = {
  0x00, 0x00, 0xCF, 0xCF, 0x00, 0x00, 0x00,
  0x00, 0xCF, 0x0F, 0x0F, 0x8A, 0x00, 0x00,
  0x00, 0x8F, 0x0F, 0x0F, 0x4F, 0x00, 0x00,
  0x45, 0x0F, 0x0F, 0x0F, 0x4F, 0x00, 0x00,
  0x45, 0x0F, 0x0F, 0x0F, 0x0F, 0x8A, 0x00,
  0xCF, 0x0F, 0x0F, 0x0F, 0x0F, 0x8A, 0x00,
  0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x4F, 0x00,
  0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x8A,
  0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x8A,
  0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x4F,
  0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x4F,
  0xCF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x4F,
  0x45, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x4F,
  0x45, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x4F,
  0x00, 0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x4F,
  0x00, 0x8F, 0x0F, 0x0F, 0x0F, 0x0F, 0x8A,
  0x00, 0x45, 0x0F, 0x0F, 0x0F, 0x4F, 0x00,
  0x00, 0x00, 0xCF, 0xCF, 0xCF, 0xCF, 0x00,
};

void wait_vsync()
{
  // Read PPI port B (0xF5).
  // Bit 0 = 1 if vsync is active.
  #asm
  ld b,0xf5
  .wait_vsync_loop
  in a,(c)
  rra
  jp nc,wait_vsync_loop
  #endasm
}

void erase_player_slow(int x, int y)
{
  for (int py = y; py < y + PLAYER_HEIGHT; py++)
  {
    unsigned char* address = 0xC000 + ((py / 8) * 80) + ((py % 8) * 2048) + x;
    for (int px = 0; px <= PLAYER_WIDTH; px++)
    {
      *address++ = 0;
    }
  }
}

void erase_player(int x, int y)
{
  #asm
  pop bc // return address
  pop hl // y
  pop de // x
  push de
  push hl
  push bc
  push de // x 2nd on the stack
  push hl // y on top of the stack
  ld b, PLAYER_HEIGHT
.erase_player_y_loop
  ld hl, 0xC000 // Screen address
  pop de // y
  push de
  // Round for division by 8:
  // y8 = 8*(y/8)
  ld a, 0xF8
  and e
  ld e, a
  // Address += (y8 * 8) + (y8 * 2)
  or a // clear carry
  rl e
  rl d
  add hl, de
  or a // clear carry
  rl e
  rl d
  or a // clear carry
  rl e
  rl d
  add hl, de
  // Address += (y % 8) * 2048
  // As there are only 7 values, a table could be faster
  pop de
  push de
  ld a, 7
  and e
  ld d, a
  xor a  // clear carry and a=0
  ld e, a
  // No need to care about carry after as it's <8
  rl d
  rl d
  rl d
  add hl, de
  // address += x
  pop ix
  pop de
  push de
  push ix
  add hl, de
  // Repeat PLAYER_WIDTH times
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  // inc y
  pop hl
  inc hl
  push hl
  djnz erase_player_y_loop
  pop de
  pop de
  #endasm
}

void draw_player_slow(int x, int y)
{
  int i = 0;
  for (int py = y; py < y + PLAYER_HEIGHT; py++)
  {
    unsigned char* address = 0xC000 + ((py / 8) * 80) + ((py % 8) * 2048) + x;
    for (int px = 0; px < PLAYER_WIDTH; px++)
    {
      *address++ = player_sprite[i];
      i++;
    }
  }
}

void draw_player(int x, int y, unsigned char* sprite)
{
  #asm
  pop bc // return address
  pop ix
  pop hl // y
  pop de // x
  push de
  push hl
  push ix
  push bc
  push ix // sprite 3rd on the stack
  push de // x 2nd on the stack
  push hl // y on top of the stack
  ld b, PLAYER_HEIGHT
.draw_player_y_loop
  ld hl, 0xC000 // Screen address
  pop de // y
  push de
  // Round for division by 8:
  // y8 = 8*(y/8)
  ld a, 0xF8
  and e
  ld e, a
  // Address += (y8 * 8) + (y8 * 2)
  or a // clear carry
  rl e
  rl d
  add hl, de
  or a // clear carry
  rl e
  rl d
  or a // clear carry
  rl e
  rl d
  add hl, de
  // Address += (y % 8) * 2048
  // As there are only 7 values, a table could be faster
  pop de
  push de
  ld a, 7
  and e
  ld d, a
  xor a  // clear carry and a=0
  ld e, a
  // No need to care about carry after as it's <8
  rl d
  rl d
  rl d
  add hl, de
  // address += x
  pop ix // y
  pop de // x
  pop iy // sprite
  push iy
  push de
  push ix
  add hl, de
  push iy
  pop de
  // Repeat PLAYER_WIDTH times
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  // inc y
  pop hl
  inc hl
  push hl

  pop ix
  pop iy
  pop hl
  add hl, PLAYER_WIDTH
  push hl
  push iy
  push ix

  djnz draw_player_y_loop
  pop de
  pop de
  pop de
  #endasm
}

void erase_missile(int x, int y)
{
  #asm
  pop bc // return address
  pop hl // y
  pop de // x
  push de
  push hl
  push bc
  push de // x 2nd on the stack
  push hl // y on top of the stack
  ld b, MISSILE_HEIGHT
.erase_missile_y_loop
  ld hl, 0xC000 // Screen address
  pop de // y
  push de
  // Round for division by 8:
  // y8 = 8*(y/8)
  ld a, 0xF8
  and e
  ld e, a
  // Address += (y8 * 8) + (y8 * 2)
  or a // clear carry
  rl e
  rl d
  add hl, de
  or a // clear carry
  rl e
  rl d
  or a // clear carry
  rl e
  rl d
  add hl, de
  // Address += (y % 8) * 2048
  // As there are only 7 values, a table could be faster
  pop de
  push de
  ld a, 7
  and e
  ld d, a
  xor a  // clear carry and a=0
  ld e, a
  // No need to care about carry after as it's <8
  rl d
  rl d
  rl d
  add hl, de
  // address += x
  pop ix
  pop de
  push de
  push ix
  add hl, de
  // Repeat MISSILE_WIDTH times
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  // inc y
  pop hl
  inc hl
  push hl
  djnz erase_missile_y_loop
  pop de
  pop de
  #endasm
}

void draw_missile(int x, int y, unsigned char* sprite)
{
  #asm
  pop bc // return address
  pop ix
  pop hl // y
  pop de // x
  push de
  push hl
  push ix
  push bc
  push ix // sprite 3rd on the stack
  push de // x 2nd on the stack
  push hl // y on top of the stack
  ld b, MISSILE_HEIGHT
.draw_missile_y_loop
  ld hl, 0xC000 // Screen address
  pop de // y
  push de
  // Round for division by 8:
  // y8 = 8*(y/8)
  ld a, 0xF8
  and e
  ld e, a
  // Address += (y8 * 8) + (y8 * 2)
  or a // clear carry
  rl e
  rl d
  add hl, de
  or a // clear carry
  rl e
  rl d
  or a // clear carry
  rl e
  rl d
  add hl, de
  // Address += (y % 8) * 2048
  // As there are only 7 values, a table could be faster
  pop de
  push de
  ld a, 7
  and e
  ld d, a
  xor a  // clear carry and a=0
  ld e, a
  // No need to care about carry after as it's <8
  rl d
  rl d
  rl d
  add hl, de
  // address += x
  pop ix // y
  pop de // x
  pop iy // sprite
  push iy
  push de
  push ix
  add hl, de
  push iy
  pop de
  // Repeat MISSILE_WIDTH times
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  // inc y
  pop hl
  inc hl
  push hl

  pop ix
  pop iy
  pop hl
  add hl, MISSILE_WIDTH
  push hl
  push iy
  push ix

  djnz draw_missile_y_loop
  pop de
  pop de
  pop de
  #endasm
}

void erase_enemy(int x, int y)
{
  #asm
  pop bc // return address
  pop hl // y
  pop de // x
  push de
  push hl
  push bc
  push de // x 2nd on the stack
  push hl // y on top of the stack
  ld b, ENEMY_HEIGHT
.erase_enemy_y_loop
  ld hl, 0xC000 // Screen address
  pop de // y
  push de
  // Round for division by 8:
  // y8 = 8*(y/8)
  ld a, 0xF8
  and e
  ld e, a
  // Address += (y8 * 8) + (y8 * 2)
  or a // clear carry
  rl e
  rl d
  add hl, de
  or a // clear carry
  rl e
  rl d
  or a // clear carry
  rl e
  rl d
  add hl, de
  // Address += (y % 8) * 2048
  // As there are only 7 values, a table could be faster
  pop de
  push de
  ld a, 7
  and e
  ld d, a
  xor a  // clear carry and a=0
  ld e, a
  // No need to care about carry after as it's <8
  rl d
  rl d
  rl d
  add hl, de
  // address += x
  pop ix
  pop de
  push de
  push ix
  add hl, de
  // Repeat ENEMY_WIDTH times
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  inc hl
  ld (hl), 0
  // inc y
  pop hl
  inc hl
  push hl
  djnz erase_enemy_y_loop
  pop de
  pop de
  #endasm
}

void draw_enemy(int x, int y, unsigned char* sprite)
{
  #asm
  pop bc // return address
  pop ix
  pop hl // y
  pop de // x
  push de
  push hl
  push ix
  push bc
  push ix // sprite 3rd on the stack
  push de // x 2nd on the stack
  push hl // y on top of the stack
  ld b, ENEMY_HEIGHT
.draw_enemy_y_loop
  ld hl, 0xC000 // Screen address
  pop de // y
  push de
  // Round for division by 8:
  // y8 = 8*(y/8)
  ld a, 0xF8
  and e
  ld e, a
  // Address += (y8 * 8) + (y8 * 2)
  or a // clear carry
  rl e
  rl d
  add hl, de
  or a // clear carry
  rl e
  rl d
  or a // clear carry
  rl e
  rl d
  add hl, de
  // Address += (y % 8) * 2048
  // As there are only 7 values, a table could be faster
  pop de
  push de
  ld a, 7
  and e
  ld d, a
  xor a  // clear carry and a=0
  ld e, a
  // No need to care about carry after as it's <8
  rl d
  rl d
  rl d
  add hl, de
  // address += x
  pop ix // y
  pop de // x
  pop iy // sprite
  push iy
  push de
  push ix
  add hl, de
  push iy
  pop de
  // Repeat ENEMY_WIDTH times
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  inc hl
  inc de
  ld a, (de)
  ld (hl), a
  // inc y
  pop hl
  inc hl
  push hl

  pop ix
  pop iy
  pop hl
  add hl, ENEMY_WIDTH
  push hl
  push iy
  push ix

  djnz draw_enemy_y_loop
  pop de
  pop de
  pop de
  #endasm
}

unsigned char* draw_star(int x, int y_addr, unsigned char color)
{
  unsigned char* address = y_addr + (x >> 1);
  if ((x & 1) == 0)
  {
    color = color << 1;
    *address = (*address & 0x55) | color;
  }
  else
  {
    *address = (*address & 0xAA) | color;
  }
  return address;
}

void erase_star(int x, unsigned char* y_addr)
{
  unsigned char* addr = y_addr + (x >> 1);
  if ((x & 1) == 0)
  {
    *addr = (*addr & 0x55);
  }
  else
  {
    *addr = (*addr & 0xAA);
  }
}

void fillscreen(unsigned char color)
{
  color |= color << 1;
  unsigned char *pScreen = (unsigned char *)0xC000;
  for(int nByte = 0; nByte < 0x3FFF; nByte++)
	{
		*pScreen++ = color;
	}
}

struct star {
  int x, y;
  unsigned char* y_addr;
  unsigned char* addr;
  int speed;
  unsigned char color;
} stars[NB_STARS];

void reinit_star(int i)
{
  int r = rand();
  stars[i].x = SCREEN_WIDTH;
  stars[i].y = r % (SCREEN_HEIGHT-MIN_Y) + MIN_Y;
  stars[i].y_addr = 0xC000 + ((stars[i].y / 8) * 80) + ((stars[i].y % 8) * 2048);
  switch ((r >> 1) % 5)
  {
    case 0:
      stars[i].color = 0x40;
      stars[i].speed = 1;
      break;
    case 1:
      stars[i].color = 0x04;
      stars[i].speed = 2;
      break;
    case 2:
      stars[i].color = 0x44;
      stars[i].speed = 3;
      break;
    case 3:
      stars[i].color = 0x10;
      stars[i].speed = 4;
      break;
    case 4:
      stars[i].color = 0x50;
      stars[i].speed = 5;
      break;
    case 5:
      stars[i].color = 0x14;
      stars[i].speed = 6;
      break;
  }
}

void init_stars()
{
  for (int i = 0; i < NB_STARS; i++)
  {
    reinit_star(i);
    stars[i].x = rand() % SCREEN_WIDTH;
  }
}

unsigned long int gettime()
{
  #asm
  EXTERN firmware
  call firmware
  defw KL_TIME_PLEASE
  // DE & HL now contain the result
  // DE has msb, HL lsb
  // This matches the return convention of z88dk
  #endasm
}

char joystickState()
{
  #asm
	di

  // GATE_ARRAY 0x7F
  // PPI_A 0xF4
  // PPI_B 0xF5
  // PPI_C 0xF6
  // PPI_CONTROL 0xF7

  // Write 14 to PPI Port A, (This is the index of the I/O Port A register of the PSG)
	ld bc, 0xF40E
	out (c), c

  // Write register by putting C0 (bits 6 & 7 sets) to port C
	ld bc, 0xF6C0
	out (c), c

  // Deactivate write register
	out (c),0

  // Set Port A to input
	ld bc, 0xF792
	out (c), c

	dec b
  // Write matrix line for joystick 0 (line 9) into bit 3-0 of Port C
  ld a, 0x49
	out (c), a

  // Read register by putting 40 (bits 6 set & 7 reset) to port C
	//ld bc, 0xF640
	//out (c), c

  // Read matrix data from PPI port A
	ld b, 0xF4
	in a, (c)

  // Set Port A of the PPI to output (use PPI Control register),
  // (At this stage it is possible to write data to the PSG. This algorithm assumes that port A 
  //  is in this state when it starts)
	ld bc, 0xF782
	out (c), c

  // Select PSG operation: inactive, by setting bit 7="0" and bit 6="0" of PPI Port C.
  // (This stage is required by the CPC+. If it is missing the scanning operation will fail) 
	dec b
	out (c),0
	cpl
  ld l, a
	ei
  #endasm
}

struct enemy {
  int x, y;
  char speed_x, speed_y;
  char type;
} enemies[MAX_NB_ENEMIES];

void game_loop()
{
  //unsigned long int start_time = gettime();
  //unsigned int nFrames = 0;

  unsigned char lives = 3;
  unsigned int score = 0;
  unsigned int px = PLAYER_START_X;
  unsigned int py = PLAYER_START_Y;
  unsigned int old_px, old_py;
  for (int i = 0; i < MAX_NB_ENEMIES; i++) {
    enemies[i].x = MAX_ENEMY_X;
    enemies[i].y = (rand() % (MAX_ENEMY_Y-MIN_ENEMY_Y)) + MIN_ENEMY_Y;
    enemies[i].speed_x = ENEMY_X_SPEED;
    enemies[i].speed_y = 0;
    enemies[i].type = 0;
  }
  char nb_enemies = 0;
  char missile;
  unsigned int mx;
  unsigned int my;

  cpc_SetTxtCursorPos(1, 1);
  printf("$:       \xE4:    L:   ");
  cpc_SetTxtCursorPos(3, 1);
  printf("%d ", score);
  cpc_SetTxtCursorPos(12, 1);
  printf("%d ", lives);
  cpc_SetTxtCursorPos(18, 1);
  printf("%d ", nb_enemies);

  int nFrames = 0;
  for (;;)
  {
    for (int i = 0; i < NB_STARS; i++)
    {
      struct star* s = &stars[i];
      // Erase the star at the previous position
      //putpixel_m0(stars[i].x, stars[i].y, 0x00);
      //erase_star(stars[i].x, stars[i].y_addr);
      // Incorrect (erases two pixels) but good enough!
      *s->addr = 0;

      s->x -= s->speed;

      if (s->x >= 0)
      {
        // Draw the star at the new position
        //putpixel_m0(stars[i].x, stars[i].y, stars[i].color);
        s->addr = draw_star(s->x, s->y_addr, s->color);
      }
      else
      {
        reinit_star(i);
      }
    }
    char input = joystickState();
    char moved = 0;
    old_px = px;
    old_py = py;
    if (input & 0x01)  // UP
    {
      moved = 1;
      if (py > MIN_PLAYER_Y + PLAYER_Y_SPEED) {
        py -= PLAYER_Y_SPEED;
      } else {
        py = MIN_PLAYER_Y;
      }
    }
    if (input & 0x02)  // DOWN
    {
      moved = 1;
      if (py < MAX_PLAYER_Y - PLAYER_Y_SPEED) {
        py += PLAYER_Y_SPEED;
      } else {
        py = MAX_PLAYER_Y;
      }
    }
    if (input & 0x04)  // LEFT
    {
      moved = 1;
      if (px > PLAYER_X_SPEED) {
        px -= PLAYER_X_SPEED;
      } else {
        px = 0;
      }
    }
    if (input & 0x08)  // RIGHT
    {
      moved = 1;
      if (px < MAX_PLAYER_X - PLAYER_X_SPEED) {
        px += PLAYER_X_SPEED;
      } else {
        px = MAX_PLAYER_X;
      }
    }
    if ((input & 0x20) && !missile)  // FIRE 1
    {
      missile_sound();
      missile = 1;
      mx = px + PLAYER_WIDTH-2;
      my = py + PLAYER_HEIGHT/2 - 1;
    }
    if (moved) {
      wait_vsync();
      erase_player(old_px, old_py);
    }
    draw_player(px, py, player_sprite);
    if (missile) {
      if (missile > 1) {
        erase_missile(mx, my);
      } else {
        missile = 2;
      }
      mx += MISSILE_SPEED;
      if (mx >= MAX_MISSILE_X) {
        missile = 0;
      } else {
        draw_missile(mx, my, missile_sprite);
      }
    }
    int midx = px + (PLAYER_WIDTH >> 1);
    int midy = py + (PLAYER_HEIGHT >> 1);
    int tipx = px + PLAYER_WIDTH;
    int lowy = py + PLAYER_HEIGHT;
    for (char i = 0; i < nb_enemies; i++)
    {
      struct enemy *e = &enemies[i];
      int old_ex = e->x;
      unsigned int old_ey = e->y;
      e->x += e->speed_x;
      e->y += e->speed_y;
      if (e->x < 0) {
        e->x = MAX_ENEMY_X;
        e->y = (rand() % (MAX_ENEMY_Y-MIN_ENEMY_Y)) + MIN_ENEMY_Y;
      }
      erase_enemy(old_ex, old_ey);
      draw_enemy(e->x, e->y, enemy_sprite);
      if (missile && my >= e->y && my <= e->y + ENEMY_HEIGHT && mx >= e->x && mx <= e->x + ENEMY_WIDTH) {
        explode_sound();
        score += e->x;
        cpc_SetTxtCursorPos(3, 1);
        printf("%d ", score);
        missile = 0;
        erase_missile(mx, my);
        erase_enemy(e->x, e->y);
        e->x = MAX_ENEMY_X;
        e->y = (rand() % (MAX_ENEMY_Y-MIN_ENEMY_Y)) + MIN_ENEMY_Y;
      }
      if ((lowy >= e->y && py <= (e->y + ENEMY_HEIGHT) && midx >= e->x && px <= (e->x + ENEMY_WIDTH)) ||
          (midy >= e->y && midy <= (e->y + ENEMY_HEIGHT) && tipx >= e->x && midx <= (e->x + ENEMY_WIDTH))) {
        crash_sound();
        erase_player(px, py);
        erase_enemy(e->x, e->y);

        lives -= 1;
        if (lives == 0) return;
        cpc_SetTxtCursorPos(12, 1);
        printf("%d ", lives);
        px = PLAYER_START_X;
        py = PLAYER_START_Y;
        e->x = MAX_ENEMY_X;
        e->y = (rand() % (MAX_ENEMY_Y-MIN_ENEMY_Y)) + MIN_ENEMY_Y;
      }
    }
    nFrames += 1;
    if ((nFrames & 63) == 63) 
    {
      if (nb_enemies < MAX_NB_ENEMIES) {
        nb_enemies += 1;
        cpc_SetTxtCursorPos(18, 1);
        printf("%d ", nb_enemies);
      }
    }
    /*
    if (gettime() - start_time >= 300)
    //if (nFrames == 0x3F)
    {
      unsigned long int end_time = gettime();
      cpc_SetTxtCursorPos(1, 1);
      // Time is in 300th of seconds
      // Computing FPS with floating point is very expensive, skewing the result.
      //double_t fps = (300.0*nFrames) / (end_time-start_time);
      //printf("%f  \n", fps);
      //printf("%lu  ", end_time-start_time);
      // ~FPS if checking gettime() - start_time >= 300
      printf("%u  ", nFrames);
      start_time = end_time;
      nFrames = 0;
    }
    */
  }
}

void black_palette()
{
  for (int c = 0; c < 16; c++)
  {
    cpc_set_palette(c, 0);
  }
}

void game_palette()
{
  // For the stars
  cpc_set_palette(0, 0);  // Black
  cpc_set_palette(1, 9);  // Dark green for slow stars
  cpc_set_palette(2, 18); // Bright green for fast stars
  cpc_set_palette(3, 21); // Lime
  cpc_set_palette(4, 19); // Sea green
  cpc_set_palette(5, 22); // Pastel green
  // For the player
  cpc_set_palette(6, 6);  // Bright Red
  cpc_set_palette(7, 15); // Orange
  cpc_set_palette(8, 24); // Yellow
  // For the enemies
  //cpc_set_palette(9, 3); // Red (Dark)
  cpc_set_palette(9, 1); // Blue (Dark)
  //cpc_set_palette(10, 12); // Yellow
  cpc_set_palette(10, 12); // Yellow
  cpc_set_palette(11, 13); // Grey
}

void game_over()
{
  game_over_sound();
  cpc_SetTxtCursorPos(5, 10);
  printf("            ");
  cpc_SetTxtCursorPos(5, 11);
  printf(" GAME OVER! ");
  cpc_SetTxtCursorPos(5, 12);
  printf("            ");

  while (getk()) {
    msleep(20);
  }
  msleep(1000);
  fgetc_cons();
  // TODO: Highscores
}

void main()
{
  cpc_SetBorder(0);
  black_palette();
  cpc_setmode(0);

  sound_setup();

  for (;;)
  {
    black_palette();
    fillscreen(0x00); // Black screen
    game_palette();

    // TODO: Game intro

    init_stars();
    game_loop();

    game_over();
  }
}
