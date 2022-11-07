#include <stdio.h>
#include <cpc.h>

// Print Pascal's triangle
// We print as many line as we can fit on the screen.
// This requires using long int otherwise this overflows on line 18 (or 19 with
// unsigned).
// We format so that numbers are aligned (which depends on the number of lines,
// here 6 digits).
// On each line, we only print what fits on the screen, which is 10 numbers, so
// the triangle is truncated on the right starting at line 11.

// We compute elements recursively, with memoization. The non-memoized version
// is extremely slow.

// We use a "square" cache. We're wasting half of it as we don't print the end
// of lines (on top of the half which is lost anyway by the fact that it's a
// triangle, so j>i means the result is 0).
#define CACHE_SIZE 22

int cache_initialized = 0;
long int cache[CACHE_SIZE][CACHE_SIZE];

void cache_init()
{
  int i, j;
  for (i = 0; i < CACHE_SIZE; i++)
  {
    for (j = 0; j < CACHE_SIZE; j++)
    {
      cache[i][j] = -1;
    }
  }
  cache_initialized = 1;
}

// Memoized version, this is, of course, much faster!
long int pascal_cache(long int i, long int j)
{
  if (!cache_initialized) cache_init();
  if (i < 0 || j < 0) return 0;
  if (i == 0 && j == 0) return 1;
  if (i == 0 && j > 0) return 0;
  if ((i < CACHE_SIZE) && (j < CACHE_SIZE) && cache[i][j] != -1) return cache[i][j];
  long int r = pascal_cache(i-1, j) + pascal_cache(i-1, j-1);
  if ((i < CACHE_SIZE) && (j < CACHE_SIZE)) cache[i][j] = r;
  return r;
}

// Basic version, awfully slow
long int pascal(long int i, long int j)
{
  if (i < 0 || j < 0) return 0;
  if (i == 0 && j == 0) return 1;
  if (i == 0 && j > 0) return 0;
  return pascal(i-1, j) + pascal(i-1, j-1);
}

main()
{
  cpc_setmode(2);

  printf("Pascal triangle\n\n");

  int i, j;

  for (i = 0; i < 22; i++)
  {
    for (j = 0; j < 11; j++)
    {
      long int p = pascal_cache(i, j);
      if (p != 0) printf("%6ld\t", p);
    }
    printf("\n");
  }

  fgetc_cons();
}
