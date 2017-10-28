# tiny-bignum-c


Small portable multiple-precision unsigned integer arithmetic in C.

Uses an array of `uint8_t`, `uint16_t` or `uint32_t` as underlying data-type utilizing all bits in each word.

The number-base is 0xFF, 0xFFFF or 0xFFFFFFFF depending on chosen word-size - see the header file `bn.h` for clarification.

No dynamic memory management is utilized, and `stdio.h` is only used for testing functions parsing to and from hex-strings.

Run `make all test` for examples of usage and for some random testing.

The test suite uses Python's integer implementation for verification (which is GMP underneath).


