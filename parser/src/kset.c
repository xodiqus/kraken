
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

    struct KSet* r = malloc(sizeof(struct KSet));
    *r = (struct KSet){ .items = NULL, .length = 0 }; 
    
    struct KString srs = drop_until(span_KString(in, strlen(in)), "{");
    srs = rdrop_until(srs, "}");

    srs = trim_KString(srs);

    struct Splited splited = split_with(srs, ","); 
    r->length = splited.count;

    if (r->length != 0)
    {
        r->items = malloc(r->length * sizeof(struct KAny));    

        for (size_t i = 0; i < r->length; ++i)
        {
            struct KAny any = create_KAny(splited.items[i]);
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