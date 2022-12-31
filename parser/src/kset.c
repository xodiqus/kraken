
#include "kset.h"

#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <assert.h>
#include <ctype.h>

#include "kstring.h"
#include "kutils.h"

struct KAny create_KAny(struct KString in)
{
    assert(in.len != 0);

    struct KAny r; 

    if (isdigit(in.val[0])){
        char buf[1024];
        memcpy(buf, in.val, in.len); 

        r.type = CT_U8;
        r.u64  = atoll(buf); /*stub*/
    } else {
        r.type = CT_Const;
        r.cnst = malloc(sizeof(struct KString));
       *r.cnst = copy_KString(in);
    }
    
    return r;
}

struct KSet parse_KSet(struct KContext* c, const char* in, struct KError *err)
{
    assert(c != NULL);

    struct KSet r = { .items = NULL, .length = 0 }; 
    if (err != NULL) *err = no_KError();
    
    struct KString srs = drop_until(span_KString(in, strlen(in)), "{");
    srs = rdrop_until(srs, "}");

    srs = trim_KString(srs);

    struct Splited splited = split_with(srs, ","); 
    r.length = splited.count;

    if (r.length != 0)
    {
        r.items = malloc(r.length * sizeof(struct KAny));    

        for (size_t i = 0; i < r.length; ++i)
        {
            r.items[i] = create_KAny(splited.items[i]);
        }
    } 

    free(splited.items);
    return r;
}

void free_KSet(const struct KSet* s)
{
    assert(s != NULL);

    for (size_t i = 0; i < s->length; ++i)
    {
        free(s->items);
    }
}