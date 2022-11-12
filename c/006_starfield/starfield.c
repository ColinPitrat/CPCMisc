#include <cpc.h>
#include <stdio.h>
#include <stdlib.h>

#define TXT_SET_CURSOR 0xbb75
#define KL_TIME_PLEASE 0xBD0D

#define NB_STARS 80

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
  unsigned char* address = 0xC000 + ((y / 8) * 80) + ((y % 8) * 2048) + (x >> 1);
  unsigned char mask = 0xAA;
  if ((x & 1) == 0)
  {
    color = color << 1;
    mask = mask >> 1;
  }
  *address = (*address & mask) | color;
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
  stars[i].x = 160;
  stars[i].y = r % 200;
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
    stars[i].x = rand() % 160;
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

void main_loop()
{
  //unsigned long int start_time = gettime();
  //unsigned int nFrames = 0;

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
    /*
    nFrames += 1;
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

void main()
{
  unsigned int nByte = 0;

  cpc_setmode(0);
  fillscreen(0x00); // Black screen
  cpc_SetBorder(0);

  cpc_set_palette(0, 0);  // Black
  cpc_set_palette(1, 9);  // Dark green for slow stars
  cpc_set_palette(2, 18); // Bright green for fast stars
  cpc_set_palette(3, 21); // Lime
  cpc_set_palette(4, 19); // Sea green
  cpc_set_palette(5, 22); // Pastel green
  cpc_set_palette(6, 26); // Bright white - looks weird

  init_stars();

  main_loop();
}
