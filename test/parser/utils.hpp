#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS

extern "C" {
    #include "parser/kstring.h"
}

#include <string>
#include <string_view>
#include <array>
using std::operator""sv, std::operator""s;

inline std::string_view to_sv(KString in)
{
    return std::string_view {
        in.val,
        in.len
    };
}

inline std::string_view to_sv(KString* in)
{
    assert(in != nullptr);

    return to_sv(*in);
}

#endif