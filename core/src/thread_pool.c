#include "thread_pool.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "kdebug.h"


const char* Task_name(const char *format, ...) {
    assert(format != NULL);

#ifdef KRAKEN_DEBUG

    char* name = malloc(100);
    assert(name != NULL);

    va_list args;
    va_start(args, format);

    vsprintf(name, format, args);

    va_end(args);

    return name;

#else

    return NULL;

#endif
}

void TaskQueue_init(TaskQueue *r) {
    assert(r != NULL);

    r->len = 0;
    r->first = NULL; 
    r->last = NULL;

    const int t = mtx_init(&r->mtx, mtx_plain);
    assert(t == thrd_success);
}

void TaskQueue_destroy(TaskQueue* const q) {    
    assert(q != NULL);
    
    mtx_destroy(&q->mtx);

    if (q->len == 0) {
        return;
    }

    TaskQueue_Node *node = q->first, *temp;

    while (node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }
}

bool TaskQueue_is_empty(TaskQueue* const q) {
    assert(q != NULL);

    mtx_lock(&q->mtx);
    const bool r = q->len == 0;
    if (r) {
        assert(q->first == NULL);
        assert(q->last == NULL);
    }
    mtx_unlock(&q->mtx);

    return r;
}

void TaskQueue_push(TaskQueue* q, Task t) {
    assert(q != NULL);

    TaskQueue_Node* const new = malloc(sizeof (TaskQueue_Node));
    new->task = t;
    new->next = NULL;

    mtx_lock(&q->mtx);
    if (q->len == 0) {
        q->first = new;
        q->last = new;
    } else {
        q->last->next = new;
        q->last = new;
    }
    ++q->len;
    mtx_unlock(&q->mtx);
}

Task TaskQueue_pop(TaskQueue* const q) {
    assert(q != NULL);
    assert(!TaskQueue_is_empty(q));

    mtx_lock(&q->mtx);
    const Task t = q->first->task;
    TaskQueue_Node* const node = q->first;

    if (q->len == 1) {
        q->first = NULL;
        q->last = NULL;
    } else {
        q->first = q->first->next;
    }

    free(node);    
    --q->len;

    mtx_unlock(&q->mtx);
    
    return t;
}

struct ThreadPool_loop_arg {
    ThreadPool* pool;
    size_t      this_thrd;
};

static int ThreadPool_loop(void* in) {
    assert(in != NULL);

    struct ThreadPool_loop_arg* arg = (struct ThreadPool_loop_arg*)in;

    ThreadPool* const pool = arg->pool;
    TaskQueue*  const tasks = &pool->tasks;
    size_t      const this_thrd = arg->this_thrd;

    free(in);

    kdebug_code(
        arg = NULL;
        in = NULL;
    )

    while (true) {
        mtx_lock(&pool->mtx);

        check_elapsed_sec( waiting_time, 
            while (TaskQueue_is_empty(tasks)) 
            {    
                if (pool->joined) {
                    mtx_unlock(&pool->mtx);
                    kdebug_log("thread %lu joins", this_thrd);
                    ++pool->ready_to_join;
                    goto exit;
                } else {
                    kdebug_log("thread %lu waits for a new task", this_thrd);
                    cnd_wait(&pool->cnd, &pool->mtx);
                }
            }
        )

        Task t = TaskQueue_pop(tasks);
        
        mtx_unlock(&pool->mtx);
        --pool->free_threads_count;

        kdebug_log("thread %lu starts [%s] (wait: %.2llf ms)", this_thrd, t.name, waiting_time*1000);
        check_elapsed_sec(work_time, t.f(t.arg));
        kdebug_log("thread %lu finishes [%s] (%.2llf sec)", this_thrd, t.name, work_time);

        kdebug_code(
            free((char*)t.name);
        )

        ++pool->free_threads_count;

        #if COLLECT_STATISTICS
            ++pool->statistics.solvedTasks[this_thrd];
            pool->statistics.spentTime[this_thrd] += work_time;
        #endif
    }

    exit:
}

ThreadPool *ThreadPool_new(size_t threads_count)
{
    ThreadPool* r = malloc(sizeof(ThreadPool));
    ThreadPool_init(r, threads_count);
    return r;
}

void ThreadPool_delete(ThreadPool *p)
{
    ThreadPool_destroy(p);
    free(p);
}

void ThreadPool_init(ThreadPool *p, size_t threads_count)
{
    assert(p != NULL);

    p->threads_len = threads_count;
    p->threads = malloc(threads_count * sizeof(thrd_t));
    TaskQueue_init(&p->tasks);
    p->joined = false;
    p->ready_to_join = 0;
    
    cnd_init(&p->cnd);
    mtx_init(&p->mtx, mtx_plain);

#if COLLECT_STATISTICS
    TPStatistics_init(&p->statistics, threads_count);
#endif

    p->free_threads_count = threads_count;

    check_elapsed_sec( all_thread_creation_time, 
        for (size_t i = 0; i < threads_count; ++i) {
            kdebug_log("create thread %lu", i);

            struct ThreadPool_loop_arg* arg = malloc(sizeof (struct ThreadPool_loop_arg));
            assert(arg != NULL);
            arg->pool = p; 
            arg->this_thrd = i;

            thrd_create(&p->threads[i], &ThreadPool_loop, arg);
        }
    )

    kdebug_log("All threads created (%.3llf ms)", all_thread_creation_time * 1000);
}

void ThreadPool_destroy(ThreadPool* const p) {
    assert(p != NULL);

    kdebug_log("destroy the thread pool. Remained tasks: %llu", (size_t)p->tasks.len);

    TaskQueue_destroy(&p->tasks);
    free(p->threads);

    mtx_destroy(&p->mtx);
    cnd_destroy(&p->cnd);

#if COLLECT_STATISTICS
    TPStatistics_destroy(&p->statistics);
#endif
}

void ThreadPool_plan(ThreadPool* const p, const Task task) {
    kdebug_log("plan [%s]", task.name);

    TaskQueue_push(&p->tasks, task);
    cnd_signal(&p->cnd);
}

void ThreadPool_join(ThreadPool* const p) {
    p->joined = true;

    check_elapsed_sec( broadcasting_to_join_time, 
        // while (p->ready_to_join != p->threads_len) {
            kdebug_log("broadcast all threads to join. Ready %lu/%lu", p->ready_to_join, p->threads_len);
            cnd_broadcast(&p->cnd);
        // }

        for (size_t i = 0; i < p->threads_len; ++i) {
            thrd_join(p->threads[i], NULL);
        }
    )

    kdebug_log("all %llu threads ready to join (%.3llf sec)", p->threads_len, broadcasting_to_join_time);
} 

size_t ThreadPool_freeThreadsCount(ThreadPool *const p) {
    assert(p != NULL);
    return p->free_threads_count;
}

size_t ThreadPool_tasksCount(ThreadPool *const p) {
    assert(p != NULL);
    return p->tasks.len;
}

const TPStatistics *ThreadPool_getStatistics(ThreadPool *const p)
{   
#if COLLECT_STATISTICS
    return &p->statistics;
#else 
    return NULL;
#endif
}

void TPStatistics_init(TPStatistics *s, size_t thread_len) {
    assert(s != NULL);

    s->thread_len = thread_len;
    
    s->cancelledTasks = calloc(thread_len, sizeof(s->cancelledTasks));
    assert(s->cancelledTasks);

    s->solvedTasks = calloc(thread_len, sizeof(s->solvedTasks));
    assert(s->solvedTasks != NULL);

    s->spentTime = calloc(thread_len, sizeof(s->spentTime));
    assert(s->spentTime != NULL);
}

void TPStatistics_destroy(TPStatistics* s) {
    assert(s != NULL);

    free(s->cancelledTasks);
    free(s->solvedTasks);
    free(s->spentTime);

    kdebug_code(
        memset(s, 0, sizeof(TPStatistics));
    );
}

char* TPStatistics_to_string(TPStatistics const* const s) {
    if (s == NULL) {
        const char s[] = "Statistics not available "
#if !COLLECT_STATISTICS
        "(COLLECT_STATISTICS = 0).";
#else
        "(input argument is NULL).";
#endif
        char* const result = malloc(sizeof(s));
        memcpy(result, s, sizeof(s));
        return result;
    }

    char buf[256];
    char* const result = malloc(256 * (3 + s->thread_len));
    assert(result != NULL);

    result[0] = '\0';
    sprintf(buf, "%6s\t | %6s\t | %6s\t | %6s\n", "Thread", "solved tasks", "cancelled", "elapsed (sec)");
    strcat(result, buf);

    u32 sum_solved = 0;
    u32 sum_cancelled = 0;
    f32 sum_spentTime = 0.0;

    for (size_t t = 0; t < s->thread_len; ++t) {
        sprintf(buf, " %6lu\t | %6u\t | %6u\t | %6.2f\n", 
            t, 
            s->solvedTasks[t],
            s->cancelledTasks[t],
            s->spentTime[t]
        );

        sum_solved += s->solvedTasks[t];
        sum_cancelled += s->cancelledTasks[t];
        sum_spentTime += s->spentTime[t];

        strcat(result, buf);
    }

    strcat(result, "Summary:\n");

    sprintf(buf, " %6lu\t | %6u\t | %6u\t | %6.2f\n", 
        s->thread_len, 
        sum_solved,
        sum_cancelled,
        sum_spentTime
    );
    strcat(result, buf);

    return result;
}