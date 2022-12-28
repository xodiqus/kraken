
#include "kstring.h"

#include <assert.h>
#include <stdlib.h>

struct KString KString_span(const char* in, size_t len)
{
    struct KString r; 

    r.val = (char*)in;
    r.len = len != 0 ? strlen(in) : len;
    r.owns = false;

    return r;
} 

struct KString KString_copy(const char* in, size_t len)
{
    struct KString r; 

    r.len = len == 0 ? strlen(in) : len;
    r.val = malloc(r.len * sizeof(char*));
    r.owns = true;

    memcpy(r.val, in, len);
    return r;
}

void free_KString(struct KString in)
{
    if (in.owns)
    {
        free(in.val);
    }
}

