#ifndef INCLUDED_KFUNCALL
#define INCLUDED_KFUNCALL

#include "common.h"

struct KFuncall {
    struct KString  name;
    struct KString* args;
    size_t          args_count;
};

struct KFuncall parse_KFuncall(struct KContext*, const char* in, struct KError*);

void free_KFuncall(struct KFuncall);

#endif