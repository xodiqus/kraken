#ifndef INCLUDED_KSET
#define INCLUDED_KSET

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "kstring.h"
#include "kcontext.h"
#include "kerror.h"


struct KAny;

struct KSet {
    struct KString          name;
    const struct KAny**     items;      // Set of items. Owner is KContext. 
    size_t                  length;     // Length of the set.
};

struct KSet parse_KSet(struct KContext*, const char* str, struct KError* err);

void free_KSet(struct KSet);

struct KString to_string_KSet(struct KSet);

#endif