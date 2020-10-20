//
// Created by sobieg on 20.10.2020.
//
#include <stdio.h>
#include "bn.h"
#include "time.h"


int main() {
    struct bn rnd, lb, hb;
    struct bn avg, denum;
    bignum_init(&avg);
    bignum_from_int(&denum, 1000);


    unsigned char buf[4096];
    time_t start = clock();
    for (int i = 0; i < 1000; i++) {
//        bignum_from_string(&lb,
//                           "aaaaaaaa",
//                           8);

        bignum_from_int(&lb, 1);

        bignum_from_string(&hb,
                           "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
                           8);
        bignum_random(&rnd, &lb, &hb, 8);
//        bignum_to_string(&rnd, buf, 4096);
        bignum_add(&avg, &rnd, &avg);
//        printf("Rnd: %s\n", buf);
    }

    bignum_div(&avg, &denum, &avg);
    bignum_to_string(&avg, buf, 4096);
    printf("avg: %s, time: %f\n", buf, (double)(clock()-start)/CLOCKS_PER_SEC);


    return 0;

}
