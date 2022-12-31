#include "kutils.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

struct KString take_until_n(struct KString in, const char* what, size_t len)
{
    struct KString r = { .val = in.val, .owns = false };

    size_t i = find_sub(in, span_KString(what, len));

    if (i == NOT_FOUND) {
        r.len = in.len;
    } else {
        r.len = i;
    }

    return r;
}

// Counts how much times the 'sub' string is included in the 'in' string.
size_t count_subs(struct KString in, struct KString sub)
{
    size_t r = 0;
    for (size_t i = 0, j = 0; i < in.len; ++i, j = 0)
    {
        for (; j < sub.len && at_KString(in, i) == at_KString(sub, j); ++i, ++j);

        if (j == sub.len) ++r;
    }   
    return r;
}

struct Splited split_with(struct KString in, const char* with)
{
    if (in.len == 0) {
        return (struct Splited) { .count = 0, .items = NULL };
    }

    struct Splited r; 
    size_t with_len = strlen(with);

    r.count = 1 + count_subs(in, span_KString(with, with_len));
    r.items = malloc(r.count * sizeof(struct KString));

    for (size_t i = 0; i < r.count; ++i)
    {
        struct KString t = take_until_n(in, with, with_len);
        r.items[i] = copy_KString(t);   
        in = drop_KRunes(in, t.len + with_len);
    } 

    return r;
}

size_t find_sub(struct KString in, struct KString sub)
{
    for (size_t i = 0, j = 0; i < in.len; ++i, j = 0)
    {
        for (; j < sub.len; ++j)
        {
            if (at_KString(in, i) != at_KString(sub, j))
            {
                break;
            }
        }

        if (j == sub.len) return i;
    }

    return NOT_FOUND;
}

size_t rfind_sub(struct KString in, struct KString sub)
{
    if (empty_KString(in) ) return NOT_FOUND;
    if (empty_KString(sub)) return in.len - 1;
    
    for (size_t i = in.len - 1, j = sub.len - 1;; --i, j = sub.len - 1)
    {
        for (;; --j)
        {
            if (at_KString(in, i) != at_KString(sub, j)){
                break;
            }

            if (j == 0) return in.len - 1 - i;
        }

        if (i == 0) break;
    }

    return NOT_FOUND;
}

struct KString take_until(struct KString in, const char* what)
{
    return take_until_n(in, what, strlen(what));
}

struct KString drop_until(struct KString in, const char* what)
{
    size_t what_len = strlen(what);
    size_t i = find_sub(in, span_KString(what, what_len));

    if (i == NOT_FOUND) {
        return span_KString(in.val, in.len);
    } else {
        return drop_KRunes(in, i + what_len);
    }
}

struct KString rdrop_until(struct KString in, const char* what)
{
    size_t what_len = strlen(what);
    size_t i = rfind_sub(in, span_KString(what, what_len));

    if (i == NOT_FOUND) {
        return span_KString(in.val, in.len);
    } else {
        return rdrop_KRunes(in, i + 1);
    }
}

