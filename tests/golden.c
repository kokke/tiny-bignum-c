#include <stdio.h>
#include "bn.h"


/* For table-defined list of tests */
struct test
{
  char op;           /* operator: plus, minus, multiply, divide */
  uint64_t a,b,c;    /* operands a, b and c - c contains expected result such that [c] = [a] [op] [b] */
};

/* Golden tests - input and expected outputs: */
static struct test oracle[] =
{
  {'+', 80, 20, 100 },
  {'+', 18, 22, 40 },
  {'+', 12, 8, 20 },
  {'+', 100080, 20, 100100 },
  {'+', 18, 559022, 559040 },
  {'+', 2000000000, 2000000000, 4000000000 },
  {'+', 0x00FFFF, 1, 0x010000 },
  {'+', 0x00FFFF00, 0x00000100, 0x01000000 },
  {'-', 1000001, 1000000, 1 },
  {'-', 42, 0, 42 },
  {'-', 101, 100, 1 },
  {'-', 242, 42, 200 },
  {'-', 1042, 0, 1042 },
  {'-', 101010101, 101010100, 1 },
  {'-', 0x010000, 1, 0x00FFFF },
//./build/test_random 1 0000000000f505c2 00000000000fffe0 0000000000e505e2
  {'-', 0xf505c2, 0x0fffe0, 0xe505e2 },
//./build/test_random 1 00000000009f735a 000000000065ffb5 00000000003973a5
  {'-', 0x9f735a, 0x65ffb5, 0x3973a5 },
//./build/test_random 1 0000000000cf7810 000000000004ff34 0000000000ca78dc
  {'-', 0xcf7810, 0x04ff34, 0xca78dc },
//./build/test_random 1 0000000000bbc55f 00000000004eff76 00000000006cc5e9
  {'-', 0xbbc55f, 0x4eff76, 0x6cc5e9 },
  {'-', 0x100000, 1, 0x0fffff },
  {'-', 0x010000, 1, 0x00ffff },
//./build/test_random 1 0000000000b5beb4 000000000001ffc4 0000000000b3bef0
  {'-', 0xb5beb4, 0x01ffc4, 0xb3bef0 },
//./build/test_random 1 0000000000707655 000000000050ffa8 00000000001f76ad
  {'-', 0x707655, 0x50ffa8, 0x1f76ad },
//./build/test_random 1 0000000000f0a990 00000000001cffd1 0000000000d3a9bf
  {'-', 0xf0a990, 0x1cffd1, 0xd3a9bf },
  {'*', 0x010203, 0x1020, 0x10407060 },
  {'*', 42, 0,   0 },
  {'*', 42, 1,   42 },
  {'*', 42, 2,   84 },
  {'*', 42, 10,  420 },
  {'*', 42, 100, 4200 },
  {'*', 420, 1000, 420000 },
  {'*', 200, 8,  1600 },
  {'*', 2, 256, 512 },
  {'*', 500, 2, 1000 },
  {'*', 500000, 2, 1000000 },
  {'*', 500, 500, 250000 },
  {'*', 1000000000, 2, 2000000000 },
  {'*', 2, 1000000000, 2000000000 },
  {'*', 1000000000, 4, 4000000000 },
  {'/', 0xFFFFFFFF, 0xFFFFFFFF, 1 },
  {'/', 0xFFFFFFFF, 0x10000, 0xFFFF },
  {'/', 0xFFFFFFFF, 0x1000, 0xFFFFF },
  {'/', 0xFFFFFFFF, 0x100, 0xFFFFFF },
  {'/', 1000000, 1000, 1000 },
  {'/', 1000000, 10000, 100 },
  {'/', 1000000, 100000, 10 },
  {'/', 1000000, 1000000, 1 },
  {'/', 1000000, 10000000, 0 },
  {'/', 28, 7, 4 },
  {'/', 27, 7, 3 },
  {'/', 26, 7, 3 },
  {'/', 25, 7, 3 },
  {'/', 24, 7, 3 },
  {'/', 23, 7, 3 },
  {'/', 22, 7, 3 },
  {'/', 21, 7, 3 },
  {'/', 20, 7, 2 },
  {'/', 0, 12, 0 },
  {'/', 10, 1, 10 },
  {'/', 0xFFFFFFFF, 1, 0xFFFFFFFF },
  {'/', 0xFFFFFFFF, 0x10000, 0xFFFF },
//./build/test_random 3 0000000000b36627 00000000000dff95 000000000000000c
  {'/', 0xb36627, 0x0dff95, 0x0c },
//./build/test_random 3 0000000000e5a18e 000000000009ff82 0000000000000016
  {'/', 0xe5a18e, 0x09ff82, 0x16 },
//./build/test_random 3 000000000045edd0 000000000004ff1a 000000000000000d
  {'/', 0x45edd0, 0x04ff1a, 0x0d },
  {'%', 8, 3, 2 },
  {'%', 1024, 1000, 24 },
  {'%', 0xFFFFFF, 1234, 985 },
  {'%', 0xFFFFFFFF, 0xEF, 0x6D },
  {'%', 12345678, 16384, 8526 },
//mikl@21972:~/c_bignum2$ ./build/test_random 8 0000000000e7a344 000000000071ffe8 000000000003a374
  {'%', 0xe7a344, 0x71ffe8, 0x03a374 },
//./build/test_random 8 0000000000a3a9a1 000000000002ff44 000000000001d149
  {'%', 0xa3a9a1, 0x2ff44, 0x1d149 },
//./build/test_random 8 0000000000c128b2 000000000060ff61 0000000000602951
  {'%', 0xc128b2, 0x60ff61, 0x602951 },
//./build/test_random 8 0000000000dc2254 0000000000517fea 0000000000392280
  {'%', 0xDC2254, 0x517FEA, 0x392280 },
//./build/test_random 8 0000000000769c99 00000000002cffda 00000000001c9ce5
  {'%', 0x769c99, 0x2cffda, 0x1c9ce5 },
//./build/test_random 8 0000000000c19076 000000000031ffd4 00000000002b90fa
  {'%', 0xc19076, 0x31ffd4, 0x2b90fa },
  {'&', 0xFFFFFFFF, 0x005500AA, 0x005500AA },
  {'&', 7, 3, 3 },
  {'&', 0xFFFFFFFF, 0, 0 },
  {'&', 0, 0xFFFFFFFF, 0 },
  {'&', 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF },
  {'|', 0xFFFFFFFF, 0, 0xFFFFFFFF },
  {'|', 0, 0xFFFFFFFF, 0xFFFFFFFF },
  {'|', 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF },
  {'|', 0x55555555, 0xAAAAAAAA, 0xFFFFFFFF },
  {'|', 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF },
  {'|', 4, 3, 7 },
  {'^', 7, 4, 3 },
  {'^', 0xFFFF, 0x5555, 0xAAAA },
  {'^', 0x5555, 0xAAAA, 0xFFFF },
  {'^', 0xAAAA, 0x5555, 0xFFFF },
  {'^', 0x0000, 0xFFFF, 0xFFFF },
  {'^', 0x5555, 0xFFFF, 0xAAAA },
  {'^', 0xAAAA, 0xFFFF, 0x5555 },
  {'p', 2, 0, 1 },
  {'p', 2, 1, 2 },
  {'p', 2, 2, 4 },
  {'p', 2, 3, 8 },
  {'p', 2, 10, 1024 },
  {'p', 2, 20, 1048576 },
  {'p', 2, 30, 1073741824 },
  {'<', 1, 0,        1},
  {'<', 1, 1,        2},
  {'<', 1, 2,        4},
  {'<', 1, 3,        8},
  {'<', 1, 4,       16},
  {'<', 1, 5,       32},
  {'<', 1, 6,       64},
  {'<', 1, 7,      128},
  {'<', 1, 8,      256},
  {'<', 1, 9,      512},
  {'<', 1, 10,    1024},
  {'<', 1, 11,    2048},
  {'<', 1, 12,    4096},
  {'<', 1, 13,    8192},
  {'<', 1, 14,   16384},
  {'<', 1, 15,   32768},
  {'<', 1, 16,   65536},
  {'<', 1, 17,  131072},
  {'<', 1, 18,  262144},
  {'<', 1, 19,  524288},
  {'<', 1, 20, 1048576},
  {'<', 0xdd, 0x18, 0xdd000000 },
  {'<', 0x68, 0x02, 0x01a0 },
  {'>', 0xf6, 1, 0x7b },
  {'>', 0x1a, 1, 0x0d },
  {'>', 0xb0, 1, 0x58 },
  {'>', 0xba, 1, 0x5d },
  {'>', 0x10, 3, 0x02 },
  {'>', 0xe8, 4, 0x0e },
  {'>', 0x37, 4, 0x03 },
  {'>', 0xa0, 7, 0x01 },
  {'>',       1,  0, 1},
  {'>',       2,  1, 1},
  {'>',       4,  2, 1},
  {'>',       8,  3, 1},
  {'>',      16,  4, 1},
  {'>',      32,  5, 1},
  {'>',      64,  6, 1},
  {'>',     128,  7, 1},
  {'>',     256,  8, 1},
  {'>',     512,  9, 1},
  {'>',    1024, 10, 1},
  {'>',    2048, 11, 1},
  {'>',    4096, 12, 1},
  {'>',    8192, 13, 1},
  {'>',   16384, 14, 1},
  {'>',   32768, 15, 1},
  {'>',   65536, 16, 1},
  {'>',  131072, 17, 1},
  {'>',  262144, 18, 1},
  {'>',  524288, 19, 1},
  {'>', 1048576, 20, 1},
};
const int ntests = sizeof(oracle) / sizeof(*oracle);



int main()
{
  struct bn sa, sb, sc, sd;
  uint32_t ia, ib, ic;
  char op;
  char buf[8192];
  int npassed = 0;
  int test_passed;

  printf("\nRunning \"golden\" tests (parsed using from_int):\n\n");

  int i;
  for (i = 0; i < ntests; ++i)
  {
    /* Copy operator + operands from oracle */
    op = oracle[i].op;
    ia = oracle[i].a;
    ib = oracle[i].b;
    ic = oracle[i].c;

    /* Initialize big-num structures: */
    bignum_init(&sd); /* init result holder */
    bignum_from_int(&sa, ia);
    bignum_from_int(&sb, ib);
    bignum_from_int(&sc, ic);

    /* Perform calculation: */
    switch (op)
    {
      case '+': bignum_add(&sa, &sb, &sd);   break;
      case '-': bignum_sub(&sa, &sb, &sd);   break;
      case '*': bignum_mul(&sa, &sb, &sd);   break;
      case '/': bignum_div(&sa, &sb, &sd);   break;
      case '%': bignum_mod(&sa, &sb, &sd);   break;
      case '&': bignum_and(&sa, &sb, &sd);   break;
      case '|': bignum_or (&sa, &sb, &sd);   break;
      case '^': bignum_xor(&sa, &sb, &sd);   break;
      case 'p': bignum_pow(&sa, &sb, &sd);   break;
      case '<': bignum_lshift(&sa, &sd, ib); break;
      case '>': bignum_rshift(&sa, &sd, ib); break;

      /* Crash program if operator is unsupported. */
      default:  require(0, "default switch-case hit");
    }

    /* Verify validity: */
    test_passed = (bignum_cmp(&sc, &sd) == EQUAL);

    /* Print status: */
    if (op == 'p')
    {
      printf("  %s pow(%u, %u) = %u \n", (test_passed ? "[ OK ]" : "[FAIL]"), ia, ib, ic);
    }
    else if ((op == '<') || (op == '>'))
    {
      printf("  %s %u %c%c %u = %u \n", (test_passed ? "[ OK ]" : "[FAIL]"), ia, op, op, ib, ic);
    }
    else
    {
      printf("  %s %u %c %u = %u \n", (test_passed ? "[ OK ]" : "[FAIL]"), ia, op, ib, ic);
    }

    if (test_passed)
    {
      npassed += 1;
    }
    else
    {
      bignum_to_string(&sa, buf, sizeof(buf));
      printf("    a = %s \n", buf);
      bignum_to_string(&sb, buf, sizeof(buf));
      printf("    b = %s \n", buf);
      bignum_to_string(&sc, buf, sizeof(buf));
      printf("    c = %s \n", buf);
      bignum_to_string(&sd, buf, sizeof(buf));
      printf("    d = %s \n", buf);
      printf("\n");
    }
  }

  printf("\n%d/%d tests successful.\n", npassed, ntests);


  printf("\n");

  return (ntests - npassed); /* 0 if all tests passed */
}

