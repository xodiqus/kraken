
#include "kset.h"

#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <assert.h>
#include <ctype.h>

#include <kstring.h>

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
       *r.cnst = copy_KString(in.val, in.len);
    }
    
    return r;
}

// Example: split("12, 34 ,x, 5,", 12) -> ["12", "34", "x", "5"]
// One malloc for the array.
static struct KString* split_items(const char* in, size_t len, uint64_t* count)
{
    *count = 0;

    for (size_t i = 0; i < len; ++i)
    {
        if (in[i] == ',') ++*count; 
    }

    struct KString* r = malloc(*count * sizeof(struct KString));
    bool reading = false;

    for (size_t i = 0, item_n = 0; i < len && item_n < *count; ++i)
    { 
        if (in[i] == ',') {
            reading = false;
            ++item_n;
        }   

        if (!reading && in[i] != ',') { 
            reading = true;
            r[item_n].val = (char*)in + i; 
            r[item_n].owns= false;
            r[item_n].len = 0;
        }

        if (reading) {
            r[item_n].len++;
        }
    }

    return r;
}

struct KSet parse_KSet(struct KContext* c, const char* in, struct KError *err)
{
    assert(c != NULL);

    struct KSet r = { .items = NULL, .length = 0 }; 
    if (err != NULL) *err = no_KError();

    // regmatch_t m;
    
    // int v = regexec(&c->reg_correct_set, str, 1, &m, 0);
    
    // if (v == REG_NOMATCH)
    // {
    //     if (err != NULL) *err = "The set is not correct!";
    //     return r;
    // }

    struct KString* items = split_items(in + 1, strlen(in) - 1, &r.length); // +1 and -1 are removing '{' and '}'.

    if (r.length != 0)
    {
        r.items = calloc(r.length, sizeof(struct KAny));    

        for (int i = 0; i < r.length; ++i)
        {
            r.items[i] = create_KAny(items[i]);
        }
    }

    // struct KAny* buf = malloc(1024 * sizeof(*r.items));

    // for (r.length = 0; ; ++r.length)
    // {
    //     v = regexec(&c->reg_set_item, str, 1, &m, 0);
    //     if (v == REG_NOMATCH)
    //     {
    //         break;
    //     }

    //     const char* item  = str + m.rm_so; 
    //     const size_t len  = m.rm_eo - m.rm_so;

    //     buf[r.length] = create_KAny(item, len);

    //     str += len + 1;        
    // }   

    // if (r.length != 0)
    // {
    //     r.items = buf;
    // } 
    // else 
    // {
    //     free(buf);
    // }

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