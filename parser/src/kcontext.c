#include "kcontext.h"

#include <stdlib.h>

#include "kstring.h"
#include "kset.h"
#include "kerror.h"

struct pair {
    struct KString      name;
    struct KSet         set;
};

struct KContext {
    struct pair items[1024];
};

struct KContext* create_KContext(struct KError* err)
{
    if (err != NULL)
    {
        *err = no_KError();
    }

    return malloc(sizeof(struct KContext));
}

void free_KContext(struct KContext* in)
{
    free(in);
}

struct KSet* get_from_KContext(const char* name)
{

} 
