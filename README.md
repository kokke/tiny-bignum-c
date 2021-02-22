![CI](https://github.com/kokke/tiny-bignum-c/workflows/CI/badge.svg)
# tiny-bignum-c
# A small multiple-precision integer implementation in C
### Description
Small portable [Arbitrary-precision unsigned integer arithmetic](https://en.wikipedia.org/wiki/Arbitrary-precision_arithmetic) in C, for calculating with large numbers.

Uses an array of `uint8_t`, `uint16_t` or `uint32_t` as underlying data-type utilizing all bits in each word.

The number-base is 0x100, 0x10000 or 0x100000000 depending on chosen word-size - see the header file [bn.h](https://github.com/kokke/tiny-bignum-c/blob/master/bn.h) for clarification.

No dynamic memory management is utilized, and `stdio.h` is only used for testing functions parsing to and from hex-strings.


### Current status

Basic arithmetic (+, -, *, /, %) and bitwise operations (&, |, ^. <<, >>) plus increments, decrements and comparisons are supported. 


### Design goals
The main design goal of this library is to be small, correct, self contained and use few resources while retaining acceptable performance and feature completeness. Clarity of the code is also highly valued.


### Notable features and omissions
- Small code and binary size: ~500 SLOC, ~3kb binary for x86. Statically #define'd memory usage / allocation.
- No use of dynamic memory allocation (i.e. no calls to `malloc` / `free`).
- Randomized testing validated against Python's big-integers
- Optimal memory utilization, number base is 1 + UINT{8,16,32}_MAX.


### API
This is the data-structure used, where DTYPE is `#define`'d to `uint8_t`, `uint16_t` or `uint32_t`.
```C
struct bn
{
  DTYPE array[BN_ARRAY_SIZE];
};
```

This is the public / exported API:
```C
/* Initialization functions: */
void bignum_init(struct bn* n); /* n gets zero-initialized */
void bignum_from_int(struct bn* n, DTYPE_TMP i);
int  bignum_to_int(struct bn* n);
/* NOTE: The functions below are meant for testing mainly and expects input in hex-format and of a certain length */
/*       See the implementation for details or the test-files for examples of how to use them. */
void bignum_from_string(struct bn* n, char* str, int nbytes);
void bignum_to_string(struct bn* n, char* str, int maxsize);

/* Basic arithmetic operations: */
void bignum_add(struct bn* a, struct bn* b, struct bn* c); /* c = a + b */
void bignum_sub(struct bn* a, struct bn* b, struct bn* c); /* c = a - b */
void bignum_mul(struct bn* a, struct bn* b, struct bn* c); /* c = a * b */
void bignum_div(struct bn* a, struct bn* b, struct bn* c); /* c = a / b */
void bignum_mod(struct bn* a, struct bn* b, struct bn* c); /* c = a % b */
void bignum_divmod(struct bn* a, struct bn* b, struct bn* c, struct bn* d); /* c = a/b, d = a%b */

/* Bitwise operations: */
void bignum_and(struct bn* a, struct bn* b, struct bn* c); /* c = a & b */
void bignum_or(struct bn* a, struct bn* b, struct bn* c);  /* c = a | b */
void bignum_xor(struct bn* a, struct bn* b, struct bn* c); /* c = a ^ b */
void bignum_lshift(struct bn* a, struct bn* b, int nbits); /* b = a << nbits */
void bignum_rshift(struct bn* a, struct bn* b, int nbits); /* b = a >> nbits */

/* Special operators and comparison */
int  bignum_cmp(struct bn* a, struct bn* b);               /* Compare: returns LARGER, EQUAL or SMALLER */
int  bignum_is_zero(struct bn* n);                         /* For comparison with zero */
void bignum_inc(struct bn* n);                             /* Increment: add one to n */
void bignum_dec(struct bn* n);                             /* Decrement: subtract one from n */
void bignum_pow(struct bn* a, struct bn* b, struct bn* c); /* Calculate a^b -- e.g. 2^10 => 1024 */
void bignum_isqrt(struct bn* a, struct bn* b);             /* Integer square root -- e.g. isqrt(5) => 2 */
void bignum_assign(struct bn* dst, struct bn* src);        /* Copy src into dst -- dst := src */
```
    
### Usage

Set `BN_ARRAY_SIZE` in `bn.h` to determine the size of the numbers you want to use. Default choice is 1024 bit numbers.
Set `WORD_SIZE` to {1,2,4} to use`uint8_t`, `uint16_t` or `uint32_t`as underlying data structure.

Run `make clean all test` for examples of usage and for some random testing.


### Examples

See [`tests/factorial.c`](https://github.com/kokke/tiny-bignum-c/blob/master/tests/factorial.c) for an example of how to calculate factorial(100) or 100! (a 150+ digit number).


### FAQ
- *Q: What differentiates this library from other C big integer implementations?*

  A: Small size for one. ~500 lines of C-code compiling to 2-3kb ROM, using only modest amounts of RAM.
     Utilizing all bits by using a number base 2^{8,16,32} instead of 10 which is a usual choice.


### License
All material in this repository is in the public domain.

