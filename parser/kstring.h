
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

struct KString span_KString(const char* in, size_t len);

// one malloc
struct KString copy_KString(const char* in, size_t len);

void free_KString(struct KString);

#endif