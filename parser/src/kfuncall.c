#include "kfuncall.h"

#include <stdlib.h>
#include <string.h>

#include "kerror.h"
#include "kutils.h"

struct KFuncall parse_KFuncall(struct KContext* ctx, const char* in, struct KError* err)
{
    if (err != NULL) *err = no_KError();

    struct KFuncall r;
    struct KString srs = span_KString(in, strlen(in)); 

    struct KString t = take_until(srs, " ");
    r.name = make_KString(t.val, t.len);

    t = drop_KRunes(srs, r.name.len + 1);
    struct Splited splited = split_with(t, " ");

    r.args          = splited.items;
    r.args_count    = splited.count;

    return r;
}

void free_KFuncall(struct KFuncall in)
{
    free_KString(in.name);
    for (size_t i; i < in.args_count; ++i)
    {
        free_KString(in.args[i]);
    }
}