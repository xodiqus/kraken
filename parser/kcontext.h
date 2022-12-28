#ifndef INCLUDED_KCONTEXT
#define INCLUDED_KCONTEXT

struct KContext;
struct KError;

struct KContext* create_KContext(struct KError*); 
void free_KContext(struct KContext*);

struct KSet;
struct KSet* get_from_KContext(const char* name); 

#endif