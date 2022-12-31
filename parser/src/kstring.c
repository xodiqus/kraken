#include "kstring.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "kcontext.h"


struct KString span_KString(const KRune* in, size_t len)
{
    struct KString r; 

    r.val = (KRune*)in;
    r.len = len;
    r.owns = false;

    return r;
} 

struct KString make_KString(const KRune* in, size_t len)
{
    struct KString r; 

    r.len = len;
    r.val = malloc(r.len * sizeof(KRune));
    r.owns = true;

    memcpy(r.val, in, r.len * sizeof(KRune));
    return r;
}

struct KString copy_KString(struct KString in)
{
    return make_KString(in.val, in.len);
}

void free_KString(struct KString in)
{
    if (in.owns)
    {
        free(in.val);
    }
}

KRune at_KString(struct KString in, size_t i)
{
    assert(i < in.len);
    return in.val[i];
}

struct KString drop_KRunes(struct KString in, size_t i)
{
    struct KString r; 

    r.val = in.val + i; 
    r.len = in.len - i;
    r.owns = false;

    return r;
}

struct KString rdrop_KRunes(struct KString in, size_t i)
{
    struct KString r;

    r.len = in.len - i;
    r.owns = false;
    r.val = r.val;

    return r;
}

struct KString ltrim_KString(struct KString in)
{
    if (empty_KString(in)) return span_KString(in.val, 0);

    size_t left = 0;
    
    for (size_t i = 0; i < in.len; ++i)
    {
        if (at_KString(in, i) == ' ')
        {
            ++left;
        } else break;
    }

    return drop_KRunes(in, left);
}

struct KString rtrim_KString(struct KString in)
{
    if (empty_KString(in)) return span_KString(in.val, 0);

    size_t right = 0;

    for (size_t i = in.len - 1;; --i)
    {
        if (at_KString(in, i) == ' ')
        {
            ++right;
        } else break;

        if (i == 0) break;
    }

    return rdrop_KRunes(in, right);
}

struct KString trim_KString(struct KString in)
{
    return rtrim_KString(ltrim_KString(in));
} 

bool empty_KString(struct KString in)
{
    return in.len == 0; 
}

size_t hash_KString(struct KString in)
{
    size_t hash = 0; 

    for (size_t i = 0; i < in.len; ++i)
    {
        hash += (i+1) * (size_t)in.val[i];
    }

    return hash % KCONTEXT_HASH_TABLE_SIZE;
}