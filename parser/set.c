
#include "set.h"

#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <assert.h>

struct context {
    regex_t reg_correct_set;
    regex_t reg_set_item;
};

struct KAny create_KAny(const char* str, size_t len)
{
    struct KAny a; 

    char buf[1024];
    memcpy(buf, str, len); 

    a.type  = CT_U8;
    a.value = atoi(buf);
    
    return a;
}

struct context* create_context(const char** err)
{
    struct context* c = malloc(sizeof(struct context));

    if (regcomp(&c->reg_correct_set, "\\s*{\\s*\\([0-9],\\)*}\\s*", 0) && err != NULL)
    {
        *err = "Can't initialize reg_correct_set!";
    } 

    if (regcomp(&c->reg_set_item, "[0-9]", 0) && err != NULL)
    {
        *err = "Can't initialize reg_set_item!";
    }

    return c;
}

void free_context(struct context* c)
{
    assert(c != NULL);

    regfree(&c->reg_correct_set);

    free(c);
}

struct KSet parse_KSet(struct context* c, const char* in, const char **err)
{
    assert(c != NULL);
    const char* str = in;

    struct KSet r = { .items = NULL, .length = 0 }; 
    regmatch_t m;
    
    int v = regexec(&c->reg_correct_set, str, 1, &m, 0);
    
    if (v == REG_NOMATCH)
    {
        if (err != NULL) *err = "The set is not correct!";
        return r;
    }

    struct KAny* buf = malloc(1024 * sizeof(*r.items));

    for (r.length = 0; ; ++r.length)
    {
        v = regexec(&c->reg_set_item, str, 1, &m, 0);
        if (v == REG_NOMATCH)
        {
            break;
        }

        const char* item  = str + m.rm_so; 
        const size_t len  = m.rm_eo - m.rm_so;

        buf[r.length] = create_KAny(item, len);

        str += len + 1;        
    }   

    if (r.length != 0)
    {
        r.items = buf;
    } 
    else 
    {
        free(buf);
    }

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