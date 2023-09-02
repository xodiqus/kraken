#include "random_access_range_i32.h"

#include <assert.h>

i32 random_access_range_i32_get(const random_access_range_i32* const r, umax index) {
    assert(r != NULL);

    assert(index < r->length);

    return r->items[index];
}