#include <stdio.h>
#include <math.h>
#include <graphics.h>

#define BUFSIZE 80
#define BOOL unsigned char
#define TRUE 1
#define FALSE 0

double_t min_x = -2.0;
double_t max_x = 1.0;
double_t min_y = -1.0;
double_t max_y = 1.0;

draw_mandelbrot_at_offset(x_off, y_off, delta)
{
  for (int x = x_off; x < 640; x += delta)
  {
    double_t X = x;
    X = min_x + X * (max_x - min_x) / 640;
    for (int y = y_off; y < 200; y += delta)
    {
      double_t Y = y;
      Y = min_y + Y * (max_y - min_y) / 200;
      // Z = Z^2 + C
      // C = X + iY
      // Z = A + iB
      // Z^2 = A*A - B*B + i*2*A*B
      double_t A = X;
      double_t B = Y;
      BOOL diverges = FALSE;
      for (int i = 0; i < 20; i++)
      {
        double_t new_A = A*A - B*B + X;
        B = 2*A*B + Y;
        A = new_A;
        double_t d = A*A + B*B;
        if (d > 4.0) {
          diverges = TRUE;
          break;
        }
      }
      if (diverges) {
        plot(x, y);
      }
    }
  }
}

draw_mandelbrot()
{
  int delta = 4;
  for (int dx = 0; dx < delta; dx++)
  {
    for (int dy = 0; dy < delta; dy++)
    {
      draw_mandelbrot_at_offset(dx, dy, delta);
    }
  }
}

main()
{
  cpc_setmode(2);

  draw_mandelbrot();

  fgetc_cons();
}
