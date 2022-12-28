#ifndef INCLUDED_KERROR
#define INCLUDED_KERROR

#include <stddef.h>

enum Errnum { EN_OK = 0, };

struct KError {
    const char*     msg;
    enum Errnum     num;
};

struct KError no_KError();

#endif 