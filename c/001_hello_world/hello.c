#include <stdio.h>
#include <graphics.h>
#include <cpc.h>

#define TXT_SET_CURSOR 0xbb75

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

main()
{
  printf("Hello, world!\n");
  fgetc_cons();

  int i,x1,y1,x2,y2;
  // clg changes colors, so we prefer cpc_ClrScr
  //clg();
  //cpc_SetInk(0, 2);
  //cpc_SetInk(1, 3);
  //cpc_SetBorder(2);
  cpc_ClrScr();

  int start_x = 0;
  int end_y = 0;
  int start_y = 128+end_y;
  int end_x = 256+start_x;

  int iterations;
  for (iterations = 1; iterations <= 64; iterations *= 2) {
    cpc_ClrScr();
    cpc_setmode(0);

    cpc_SetTxtCursorPos(5, 13);
    printf("Hello, world!\n");

    int delta_x = (end_x - start_x) / iterations;
    int delta_y = (end_y - start_y) / iterations;
    for (i=0; i<=iterations; i++)
    {
      y1 = start_y + delta_y * i ;
      x2 = start_x + delta_x * i ;
      draw(start_x, y1, x2, end_y);
    }
    fgetc_cons();
  }

  fgetc_cons();
}
