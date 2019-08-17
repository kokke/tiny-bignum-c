#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "bn.h"

int mul_get_clocks(struct bn *a, struct bn *b, struct bn *c)
{
    int clocks = clock();
    bignum_mul(a, b, c);
    return clock() - clocks;
}

int mul_alt_get_clocks(struct bn *a, struct bn *b, struct bn *c)
{
    int clocks = clock();
    bignum_mul_alt(a, b, c);
    return clock() - clocks;
}

int int_test()
{
    struct bn a, b, c;
    int clocks = 0,
        clocks_alt = 0,
        num1 = 0,
        num2 = 0;
    char res[1024] = {0},
         res_alt[1024] = {0};
    bignum_init(&a);
    bignum_init(&b);
    bignum_init(&c);
    printf("method\tnum1\tnum2\tresult\tclocks\n");

    for (int i = 0; i < 10; ++i)
    {
        num1 = rand();
        num2 = rand();

        bignum_from_int(&a, num1);
        bignum_from_int(&b, num2);

        clocks = mul_get_clocks(&a, &b, &c);
        bignum_to_string(&c, res, 1024);

        clocks_alt = mul_alt_get_clocks(&a, &b, &c);
        bignum_to_string(&c, res_alt, 1024);

        printf("normal\t%d\t%d\t%s\t%d\n", num1, num2, res, clocks);
        printf("alter\t%d\t%d\t%s\t%d\n", num1, num2, res_alt, clocks_alt);
        printf("\n");
    }

    return 0;
}

int bigger_then_int_test()
{
    struct bn a, b, c;
    int clocks = 0,
        clocks_alt = 0;
    char res[1024] = {0},
         res_alt[1024] = {0},
         num1[1024] = {0},
         num2[1024] = {0};

    bignum_init(&a);
    bignum_init(&b);
    bignum_init(&c);
    printf("method\tnum1\tnum2\tresult\tclocks\n");

    for (int i = 0; i < 10; ++i)
    {
        a.array[0] = rand();
        a.array[1] = rand();
        a.array[2] = rand();
        a.array[3] = rand();

        b.array[0] = rand();
        b.array[1] = rand();
        b.array[2] = rand();
        b.array[3] = rand();

        bignum_to_string(&a, num1, 1024);
        bignum_to_string(&b, num2, 1024);

        clocks = mul_get_clocks(&a, &b, &c);
        bignum_to_string(&c, res, 1024);

        clocks_alt = mul_alt_get_clocks(&a, &b, &c);
        bignum_to_string(&c, res_alt, 1024);

        printf("normal\t%s\t%s\t%s\t%d\n", num1, num2, res, clocks);
        printf("alter\t%s\t%s\t%s\t%d\n", num1, num2, res_alt, clocks_alt);
        printf("\n");
    }

    return 0;
}

int main()
{
    srand(time(NULL));
    int_test();
    bigger_then_int_test();
    return 0;
}
