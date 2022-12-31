#include "kcontext.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "kstring.h"
#include "kset.h"
#include "kerror.h"
#include "kany.h"

struct KContext {
    struct KAny hash_table[KCONTEXT_HASH_TABLE_SIZE];
};

struct KContext* create_KContext(struct KError* err)
{
    if (err != NULL)
    {
        *err = no_KError();
    }

    struct KContext* r = malloc(sizeof(struct KContext));

    for (size_t i = 0; i < KCONTEXT_HASH_TABLE_SIZE; ++i)
    {
        r->hash_table[i].uses = false;
    }

    return r;
}

void free_KContext(struct KContext* in)
{
    for (size_t i = 0; i < KCONTEXT_HASH_TABLE_SIZE; ++i)
    {
        struct KAny* item = &in->hash_table[i];
        if (item->uses)
        {
            free_KAny(*item);
        }
    }

    free(in);
}

const struct KAny* get_from_KContext(const struct KContext* ctx, const char* name)
{
    struct KAny temp = create_KAny(span_KString(name, strlen(name)));

    size_t hash = hash_KAny(temp);
    free_KAny(temp);

    return &ctx->hash_table[hash];
} 

const struct KAny* add_to_KContext(struct KContext* ctx, struct KAny* in)
{
    assert(ctx != NULL);
    assert(in != NULL);
    assert(in->uses);

    size_t hash = hash_KAny(*in);
    struct KAny *item = &ctx->hash_table[hash];

    *item = *in;

    return item;
}
