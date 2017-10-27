#include <stdint.h>
#include <stdio.h>
#include "bn.h"

/*

Division algorithms using bitwise operations

To showcase different methods for implementing division

*/


static int divide(int dividend, int divisor)
{
  int current = 1;
  int denom = divisor;
  // This step is required to find the biggest current number which can be
  // divided with the number safely.
  while (denom <= dividend) {
    current <<= 1;
    denom <<= 1;
  }
  // Since we may have increased the denomitor more than dividend
  // thus we need to go back one shift, and same would apply for current.
  denom >>= 1;
  current >>= 1;
  int answer = 0;
  // Now deal with the smaller number.
  while (current != 0)
  {
    if (dividend >= denom)
    {
      dividend -= denom;
      answer |= current;
    }
    current >>= 1;
    denom >>= 1;
  }
  return answer;
}

void divide2(unsigned dividend, unsigned divisor, unsigned* quotient, unsigned* remainder)
{
  unsigned t, num_bits;
  unsigned q, bit, d;
  unsigned i;

  *remainder = 0;
  *quotient = 0;

  if (divisor == 0)
    return;

  if (divisor > dividend) {
    *remainder = dividend;
    return;
  }

  if (divisor == dividend) {
    *quotient = 1;
    return;
  }

  num_bits = 32;

  while ((*remainder) < divisor) {
    bit = (dividend & 0x80000000) >> 31;
    *remainder = ((*remainder) << 1) | bit;
    d = dividend;
    dividend = dividend << 1;
    num_bits--;
  }

  /* The loop, above, always goes one iteration too far.
     To avoid inserting an "if" statement inside the loop
     the last iteration is simply reversed. */
  dividend = d;
  *remainder = (*remainder) >> 1;
  num_bits++;

  for (i = 0; i < num_bits; i++) {
    bit = (dividend & 0x80000000) >> 31;
    *remainder = ((*remainder) << 1) | bit;
    t = (*remainder) - divisor;
    q = !((t & 0x80000000) >> 31);
    dividend = dividend << 1;
    *quotient = ((*quotient) << 1) | q;
    if (q) {
       *remainder = t;
     }
  }
}  /* unsigned_divide */

int main()
{
  unsigned a,b,q,r;

  a = 255;
  b = 8;

  /*
    a div b = q [with remainder r]
  */

  printf("1: %d / %d = %d \n", a,b,divide(a, b));
  divide2(a,b,&q, &r);
  printf("2: %d / %d = %d (%d) \n", a,b,q,r);


  return 0;
}


