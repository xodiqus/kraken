#ifndef KRAKEN_RANDOM_ACCESS_RANGE_INCLUDED
#define KRAKEN_RANDOM_ACCESS_RANGE_INCLUDED

#include "fundamental_types.h"

typedef struct random_access_range_i32 {
    i32* items;
    umax length;
} random_access_range_i32;

i32 random_access_range_i32_get(const random_access_range_i32*, umax index);

#endif