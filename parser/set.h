
#include <stddef.h>

enum CType {
    CT_Bool  = 1 ,
    CT_I8        ,
    CT_U8        ,
};

struct KAny {
    enum CType type; 
    void*      value;
};

struct KSet {
    struct KAny*    items;
    size_t          length;
};

struct context;

struct context* create_context(const char** err); 
void free_context(struct context*);

struct KSet parse_KSet(struct context*, const char* str, const char**err);