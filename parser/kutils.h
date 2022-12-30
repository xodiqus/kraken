#ifndef INCLUDED_KUTILS
#define INCLUDED_KUTILS

#include "common.h"
#include "kstring.h"

struct Splited {
    struct KString*     items;
    size_t              count;
};

struct Splited split_with(struct KString in, const char* with);

struct KString take_until(struct KString in, const char* what);

struct KString drop_until(struct KString in, const char* what);
struct KString rdrop_until(struct KString in, const char* what);

#define NOT_FOUND ((size_t)-1)

// Finds substring in the string 'in' and returns index, otherwise NOT_FOUND. 
size_t find_sub(struct KString in, struct KString sub);

#endif