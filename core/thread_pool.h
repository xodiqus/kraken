#ifndef KRAKEN_THREAD_POOL_INCLUDED
#define KRAKEN_THREAD_POOL_INCLUDED

#include "fundamental_types.h"
#include "task.h"

#ifndef COLLECT_STATISTICS
#define COLLECT_STATISTICS  1
#endif

typedef struct TPStatistics {
    umax    threads_count;
    u32*    solved_tasks;       // of every thread;
    u32*    cancelled_tasks;    // of every thread;
    f32*    spent_time;         // for solving all tasks of one thread;
    umax    total_tasks_count;  // number of tasks;
} TPStatistics;

#ifndef KRAKEN_IMPLICIT_DEFINITION

#include <threads.h>
#include <stdatomic.h>

typedef struct TaskQueue_Node {
    Task*                     task; 
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

    atomic_size_t busy_threads_count;
    atomic_size_t ready_to_join;

    TaskQueue   tasks;
    atomic_bool joined;

    cnd_t       cnd;
    mtx_t       mtx;

    TPStatistics statistics;
} ThreadPool;

#else 

struct ThreadPool;
struct TaskQueue;

struct ThreadPool* ThreadPool_new(size_t threads_count);
void               ThreadPool_delete(struct ThreadPool*);

#endif

void        TaskQueue_init(struct TaskQueue*);
void        TaskQueue_destroy(struct TaskQueue*);

bool        TaskQueue_is_empty(struct TaskQueue*);
void        TaskQueue_push(struct TaskQueue*, Task*);
Task*       TaskQueue_pop (struct TaskQueue*);

void        ThreadPool_init(struct ThreadPool*, size_t threads_count);
void        ThreadPool_destroy(struct ThreadPool*);

void        ThreadPool_plan(struct ThreadPool*, Task*);
void        ThreadPool_join(struct ThreadPool*);
size_t      ThreadPool_freeThreadsCount(struct ThreadPool*);
size_t      ThreadPool_tasksCount(struct ThreadPool* const);
const TPStatistics* 
            ThreadPool_getStatistics(struct ThreadPool* const);

void        TPStatistics_init(TPStatistics*, size_t thread_len);
void        TPStatistics_destroy(TPStatistics*);
char*       TPStatistics_to_string(TPStatistics const*);

#endif