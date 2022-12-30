
#ifndef INCLUDED_KSTRING
#define INCLUDED_KSTRING

#include <stdbool.h>
#include <stddef.h>

typedef char KRune;

struct KString
{
    KRune*    val;
    size_t    len;
    bool      owns; 
};

struct KString span_KString(const KRune* in, size_t len);

// one malloc
struct KString make_KString(const KRune* in, size_t len);

// Copies KString, making owner-string.
struct KString copy_KString(struct KString in);

void free_KString(struct KString);

KRune at_KString(struct KString, size_t i);

struct KString drop_KRunes(struct KString, size_t i);
struct KString rdrop_KRunes(struct KString, size_t i);

struct KString trim_KString(struct KString); 

bool empty_KString(struct KString);

#endif