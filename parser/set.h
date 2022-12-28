#ifndef INCLUDED_SET
#define INCLUDED_SET

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "kstring.h"

enum CType {
    CT_Bool  = 1 ,
    CT_I8        ,
    CT_U8        ,
    CT_Const     ,
};

struct KAny {
    enum CType type; 
    union {
        void*           ptr;
        uint64_t        u64;
        int64_t         i64;
        bool            bl;
        struct KString* cnst;
    };
};

struct KSet {
    struct KAny*    items;
    size_t          length;
};

struct context;

struct context* create_context(const char** err); 
void free_context(struct context*);

struct KSet parse_KSet(struct context*, const char* str, const char**err);

#endif