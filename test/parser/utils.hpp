#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS

extern "C" {
    #include "parser/kstring.h"
}

#include <string>
#include <string_view>

inline std::string_view to_sv(KString in)
{
    return std::string_view {
        in.val,
        in.len
    };
}

#endif