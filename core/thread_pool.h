#ifndef KRAKEN_THREAD_POOL_INCLUDED
#define KRAKEN_THREAD_POOL_INCLUDED

#include "fundamental_types.h"

typedef struct Task {
    i32 (*f)(void*);
    void* arg;

// #ifdef KRAKEN_DEBUG_MODE    
    const char* name; 
// #endif
} Task;

#ifdef KRAKEN_EXPLICIT_THREAD_POOL_DEFINITION

#include <threads.h>
#include <stdatomic.h>

typedef struct TaskQueue_Node {
    Task                      task; 
    struct TaskQueue_Node*    next;
} TaskQueue_Node;

typedef struct TaskQueue {
    atomic_size_t           len; 
    struct TaskQueue_Node  *first, *last;
    mtx_t                   mtx;
} TaskQueue;

typedef struct ThreadPool {
    thrd_t* threads;
    size_t  threads_len;

    atomic_size_t free_threads_count;
    atomic_size_t ready_to_join;

    TaskQueue   tasks;
    atomic_bool joined;

    cnd_t       cnd;
    mtx_t       mtx;
} ThreadPool;

#else 

struct ThreadPool;
struct TaskQueue_Node;
struct TaskQueue;

#endif

void        TaskQueue_init(struct TaskQueue*);
void        TaskQueue_destroy(struct TaskQueue*);

bool        TaskQueue_is_empty(struct TaskQueue*);
void        TaskQueue_push(struct TaskQueue*, Task);
Task        TaskQueue_pop (struct TaskQueue*);

struct ThreadPool* ThreadPool_create(size_t threads_count);
void        ThreadPool_init(struct ThreadPool*, size_t threads_count);
void        ThreadPool_destroy(struct ThreadPool*);

void        ThreadPool_plan(struct ThreadPool*, Task);
void        ThreadPool_join(struct ThreadPool*);
size_t      ThreadPool_freeThreadsCount(struct ThreadPool*);
size_t      ThreadPool_tasksCount(struct ThreadPool* const);

#endif