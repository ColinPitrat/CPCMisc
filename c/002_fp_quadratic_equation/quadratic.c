#include <stdio.h>
#include <math/math_cpc.h>

#define BUFSIZE 80

// Implement our own sqrt as it's not available in the math library at the
// moment.
// Not needed in the end as pow(x, 0.5) works but initially it was silently
// broken because I was not including math/math_cpc.h.
double_t sqrt(double_t x)
{
  double_t e = x;
  double_t new_e = 0;
  for (int i = 0; i < 100; i++)
  {
    new_e = (e*e + x) / (2*e);
    //printf("sqrt(%f): %d: e = %f\n",  x, i, new_e);
    if (new_e == e) return e;
    e = new_e;
  }
  return e;
}

sqrt_test()
{
  for (int i = 0; i < 20; i++)
  {
    double_t a = i;
    a = a/4.0;
    printf("sqrt(%f) = %f\n", a, sqrt(a));
  }
  fgetc_cons();
}

pow_test()
{
  for (int i = 0; i < 10; i++)
  {
    double_t a = i;
    a = a/4.0;
    //printf("%d: ", i);
    for (int j = 0; j < 10; j++)
    {
      double_t b = j;
      b = b/4.0;
      printf("%f ^ %f = %f\n", a, b, pow(a, b));
    }
    printf("\n");
  }
  fgetc_cons();
}

main()
{
  //sqrt_test();
  //pow_test();

  printf("I can solve quadratic equations of the form:\n A*x^2 + B*x + C = 0.\n\n");

  double_t a, b, c;
  char buf[BUFSIZE];
  char ch = ' ';

  while (ch != 'q' && ch != 'Q')
  {
    printf("What's A? ");
    fgets_cons(buf, BUFSIZE-1);
    sscanf(buf, "%f", &a);
    printf("\n");

    printf("What's B? ");
    fgets_cons(buf, BUFSIZE-1);
    sscanf(buf, "%f", &b);
    printf("\n");

    printf("What's C? ");
    fgets_cons(buf, BUFSIZE-1);
    sscanf(buf, "%f", &c);
    printf("\n");
    printf("\n");

    double_t delta = b*b - 4*a*c;
    //printf("Delta = %f^2 - 4*%f*%f = %f\n", b, a, c, delta);
    printf("Delta = %f\n", delta);
    printf("\n");

    if (delta < 0)
    {
      printf("No real solution.\n");
    }
    else if (delta == 0)
    {
      double_t r = -b/(2*a);
      printf("One real solution: x=%f\n", r);
      //printf("\n");
      //printf("%f*%f^2 + %f*%f + %f = %f\n", a, r, b, r, c, a*r*r+b*r+c);
    }
    else if (delta > 0)
    {
      //double_t s = sqrt(delta);
      double_t s = pow(delta, 0.5);
      double_t r1 = (-b-s)/(2*a);
      double_t r2 = (-b+s)/(2*a);
      printf("Two real solutions: x=%f and x=%f\n", r1, r2);
      //printf("\n");
      //printf("%f*%f^2 + %f*%f + %f = %f\n", a, r1, b, r1, c, a*r1*r1+b*r1+c);
      //printf("%f*%f^2 + %f*%f + %f = %f\n", a, r2, b, r2, c, a*r2*r2+b*r2+c);
    }

    printf("\n");
    printf("Press Q to quit, any other key to solve another equation.\n");
    printf("\n");
    printf("\n");
    ch = fgetc_cons();
  }
}
