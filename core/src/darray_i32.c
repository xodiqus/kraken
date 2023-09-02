#include "darray_i32.h"

#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "kdebug.h"

void darray_i32_init(darray_i32 *const a, const umax length, ...) {
    assert(a != NULL);

    a->length = length;
    a->capacity = 2 * length;

    a->items = malloc(length * sizeof(i32));
    assert(a->items != NULL);

    va_list args; 
    va_start(args, length);

    for (umax i = 0; i < length; ++i) {
        a->items[i] = va_arg(args, i32);
    }

    va_end(args);
}

void darray_i32_destroy(darray_i32 *const a) {
    assert(a != NULL);

    free(a->items);

    kdebug_code(
        memset(a, 0, sizeof(darray_i32));
    )
}

void darray_i32_push_back(darray_i32 *const a, i32 value) {
    assert(a != NULL);

    if (a->capacity <= a->length) {
        a->capacity *= 2;
        i32* const new = realloc(a->items, a->capacity * sizeof(i32));

        assert(new != NULL);

        a->items = new;
    }

    a->items[a->length] = value;
    ++a->length;
}

i32 darray_i32_get(darray_i32 *const a, umax index) {
    assert(a != NULL);

    assert(index < a->length);

    return a->items[index];
}

void darray_i32_set(darray_i32 *const a, umax index, i32 value) {
    assert(a != NULL);

    assert(index < a->length);

    a->items[index] = value;
}


