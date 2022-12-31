#ifndef INCLUDED_KANY
#define INCLUDED_KANY

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "common.h"
#include "kset.h"

enum CType {
    CT_Bool     = 1,
    CT_I8       = 3,
    CT_U8       = 5,
    CT_Const    = 7,
    CT_InitVar  = 9,
    CT_InitCst  = 11,
    CT_Return   = 13,
    CT_Funcall  = 15,
    CT_KSet     = 17,
};

struct KAny {
    union {
        enum CType  type; 
        bool        uses;
    };
    union {
        void*           as_ptr;
        uint64_t        as_u64;
        int64_t         as_i64;
        bool            as_bool;
        struct KString* as_const;
        struct KSet*    as_KSet;
    };

    struct KSet sets; // Set of sets which include this item.
};

struct KAny create_KAny(struct KString);

void free_KAny(struct KAny);

// x == y
bool eq_KAny(struct KAny x, struct KAny y);

size_t hash_KAny(struct KAny);

struct KString to_string_KAny(struct KAny);

#endif 