cmake_minimum_required(VERSION 3.17)
project(tiny-bignum-c VERSION 1.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

include_directories(randombytes .)


set(BN_SRC
        bn.c bn.h
        randombytes/randombytes.c randombytes/randombytes.h)

add_library(tiny_bignum ${BN_SRC})
set_target_properties(tiny_bignum PROPERTIES VERSION ${PROJECT_VERSION})

add_executable(random_test tests/random.c bn.h bn.c randombytes/randombytes.c randombytes/randombytes.h)
