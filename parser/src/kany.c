#include "kany.h"

#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "kset.h"

struct KAny create_KAny(struct KString in)
{
    assert(in.len != 0);

    struct KAny r; 

    if (isdigit(in.val[0])){
        char buf[1024];
        memcpy(buf, in.val, in.len); 

        r.type = CT_U8;
        r.as_u64  = atoll(buf); /*stub*/
    } else {
        r.type = CT_Const;
        r.as_const = malloc(sizeof(struct KString));
       *r.as_const = copy_KString(in);
    }
    
    return r;
}

void free_KAny(struct KAny in)
{
    switch (in.type)
    {
    case CT_Const: free(in.as_const);
    }
}

size_t hash_KAny(struct KAny in)
{
    assert(in.uses);

    switch (in.type)
    {
        case CT_KSet:   return hash_KString(in.as_KSet->name);
        case CT_Bool:   return in.as_bool;
        case CT_I8:     return in.as_i64 % KCONTEXT_HASH_TABLE_SIZE;
        case CT_U8:     return in.as_u64 % KCONTEXT_HASH_TABLE_SIZE;
        case CT_Const:  return hash_KString(*in.as_const);

        default: assert(false && "Can't compute a hash for KAny!");
    }
}

struct KString to_string_KAny(struct KAny in)
{
    switch (in.type)
    {
        case CT_KSet:   return to_string_KSet(*in.as_KSet);
        case CT_Bool:   return in.as_bool ? span_KString("true", 4) : span_KString("false", 5);
        case CT_I8:     
        {
            char* buf = malloc(19 * sizeof(char)); // MAX_INT has 19 characters.
            sprintf(buf, "%ld", in.as_i64);
            return (struct KString){ .owns = true, .len = strlen(buf), .val = buf };
        }
        case CT_U8:
        {
            char* buf = malloc(20 * sizeof(char)); // MAX_UINT has 20 characters.
            sprintf(buf, "%lu", in.as_i64);
            return (struct KString){ .owns = true, .len = strlen(buf), .val = buf };
        }   
        case CT_Const:  return span_KString(in.as_const->val, in.as_const->len);

        default: assert(false && "Can't cast KAny to string!");
    }
}