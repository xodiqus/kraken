
#include "kset.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "kstring.h"
#include "kutils.h"
#include "kany.h"

struct KSet parse_KSet(struct KContext* ctx, const char* in, struct KError *err)
{
    assert(ctx != NULL);

    if (err != NULL) *err = no_KError();
    
    struct KString srs = span_KString(in, strlen(in));

    struct KString name = take_until(srs, "=");
    name = trim_KString(name);

    srs = drop_until(srs, "{");
    srs = rdrop_until(srs, "}");

    srs = trim_KString(srs);

    struct Splited splited = split_with(srs, ","); 
    
    struct KSet* r = malloc(sizeof(struct KSet));
    *r = (struct KSet){ .items = NULL, .length = 0, .name = name }; 
    r->length = splited.count;

    if (r->length != 0)
    {
        r->items = malloc(r->length * sizeof(struct KAny));    

        for (size_t i = 0; i < r->length; ++i)
        {
            struct KAny any = create_KAny(trim_KString(splited.items[i]));
            r->items[i] = add_to_KContext(ctx, &any);
        }
    } 

    add_to_KContext(ctx, &(struct KAny){.type = CT_KSet, .as_KSet = r});

    free(splited.items);
    return *r;
}

void free_KSet(struct KSet s)
{
    free(s.items);
}

struct KString to_string_KSet(struct KSet in)
{
    struct KString r = {.owns = true, .len = in.name.len + 5}; // + 4 for " = {" and "}" at the end.
    struct KString* items = malloc(in.length * sizeof(struct KString));
    
    for (size_t i = 0; i < in.length; ++i)
    {
        items[i] = to_string_KAny(*in.items[i]);
        r.len += items[i].len; 
    }

    if (in.length != 0)
        r.len += 2 * (in.length - 1); // for ", " between each two elements.

    r.val = malloc(r.len * sizeof(KRune));

    size_t offset = in.name.len * sizeof(KRune);
    memcpy(r.val, in.name.val, offset);
    memcpy(r.val + offset, " = {", 4);
    offset += 4;

    for (size_t i = 0; i < in.length; ++i)
    {
        size_t t = items[i].len * sizeof(KRune);
        memcpy(r.val + offset, items[i].val, t);
        offset += t;
        memcpy(r.val + offset, ", ", 2);
        offset += 2;

        free_KString(items[i]);
    }

    r.val[r.len - 1] = '}';

    free(items);
    return r;    
}