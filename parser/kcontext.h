#ifndef INCLUDED_KCONTEXT
#define INCLUDED_KCONTEXT

enum { KCONTEXT_HASH_TABLE_SIZE = 100 };

struct KContext;
struct KError;
struct kAny;

struct KContext* create_KContext(struct KError*); 
void free_KContext(struct KContext*);

const struct KAny* get_from_KContext(const struct KContext*, const char* name); 

// Adds an item to a context and returns a pointer to its place.
const struct KAny* add_to_KContext(struct KContext*, struct KAny* item);

#endif