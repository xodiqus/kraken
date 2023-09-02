#ifndef KRAKEN_DARRAY_i32_INCLUDED
#define KRAKEN_DARRAY_i32_INCLUDED

#include "fundamental_types.h"


// Dynamic array works as std::vector<int32_t> from C++.
typedef struct darray_i32 {
    umax    capacity; 
    umax    length;
    i32*    items;
} darray_i32;

void darray_i32_init(darray_i32*, umax length, ...);
void darray_i32_destroy(darray_i32*);

void darray_i32_push_back(darray_i32*, i32 value);

i32  darray_i32_get(darray_i32*, umax index);
void darray_i32_set(darray_i32*, umax index, i32 value);

#endif