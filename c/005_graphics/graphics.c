#include <stdio.h>
#include <cpc.h>

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

void fillscreen(unsigned char color)
{
  color |= color << 1;
  unsigned char *pScreen = (unsigned char *)0xC000;
  for(int nByte = 0; nByte < 0x3FFF; nByte++)
	{
		*pScreen++ = color;
	}
}

// Only _really_ works for x1 & x2 even.
void fastfill_rect(int x1, int y1, int x2, int y2, unsigned char color)
{
  x1 = x1 >> 1;
  x2 = x2 >> 1;
  color |= (color << 1);
  for (int y = y1; y < y2; y++)
  {
    unsigned char* yaddress = 0xC000 + ((y >> 3) * 80) + ((y & 0x7) * 2048);
    for (int x = x1; x < x2; x++)
    {
      unsigned char* address = yaddress + x;
      *address = color;
    }
  }
}

// Naive fill rect, we can do much better!
// x1 < x2
// y1 < y2
void fill_rect(int x1, int y1, int x2, int y2, unsigned char color)
{
  for (int x = x1; x < x2; x++)
  {
    for (int y = y1; y < y2; y++)
    {
      putpixel_m0(x, y, color);
    }
  }
}

void main()
{
  unsigned int nByte = 0;

  cpc_setmode(0);

  // This modifies color 0x40 (remember order of bits!)
  cpc_set_palette(1, 9);
  // Test the 16 colors.
  unsigned char colors[16] = {
    0x00, 0x01, 0x04, 0x05,
    0x10, 0x11, 0x14, 0x15,
    0x40, 0x41, 0x44, 0x45,
    0x50, 0x51, 0x54, 0x55
  };
  for (int i = 0; i < 16; i++)
  {
    printf("   %02x\n", colors[i]);
    fastfill_rect( 0,  8*i,  8,  8*(i+1), colors[i]);
  }

  fgetc_cons();

  fastfill_rect(0, 0, 160, 200, 0x00);
 
  fgetc_cons();
  fillscreen(0x50); // Black screen
  //fillscreen(0x11); // Bright green screen
  //fillscreen(0x10); // White screen
  //fillscreen(0x01); // Cyan screen
  
  fgetc_cons();
}
