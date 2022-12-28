
#ifndef INCLUDED_KSTRING
#define INCLUDED_KSTRING

#include <string.h>
#include <stdbool.h>

struct KString
{
    char*     val;
    size_t    len;
    bool      owns; 
};

struct KString KString_span(const char* in, size_t len);

// one malloc
struct KString KString_copy(const char* in, size_t len);

void free_KString(struct KString);

#endif