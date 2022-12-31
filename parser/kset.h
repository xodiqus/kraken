#ifndef INCLUDED_KSET
#define INCLUDED_KSET

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "kstring.h"
#include "kcontext.h"
#include "kerror.h"

enum CType {
    CT_Bool  = 1 ,
    CT_I8        ,
    CT_U8        ,
    CT_Const     ,
    CT_InitVar   ,
    CT_InitCst   ,
    CT_Return    ,
    CT_Funcall   ,
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

struct InitVar {
    struct KString      name; 
    struct KAny         val;
};

typedef struct InitVar InitCst;


struct KArg {
    struct KString  name;
    struct KString  type;
};

struct KSet {
    struct KAny*    items;
    size_t          length;
};

struct KSet parse_KSet(struct KContext*, const char* str, struct KError* err);

#endif