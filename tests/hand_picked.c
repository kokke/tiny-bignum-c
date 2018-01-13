#include <stdio.h>
#include "bn.h"


int npassed = 0;
int ntests = 0;


static void test_evil(void)
{
  ntests += 1;
  /*
    Evil test case triggering infinite-loop - reported by jepler@github
    https://github.com/kokke/tiny-bignum-c/issues/2
  */
  {
    struct bn a, b, c;
    bignum_from_int(&a, 1); 
    bignum_init(&b); bignum_dec(&b); // b now holds biggest bignum
    bignum_div(&b, &a, &c);
  }
  /* test passed if it doesn't go into infinite-loop... */
  npassed += 1;
}



int main()
{
  printf("\nRunning hand-picked test cases:\n");

  test_evil();

  printf("\n%d/%d tests successful.\n", npassed, ntests);
  printf("\n");

  return 0;
}

