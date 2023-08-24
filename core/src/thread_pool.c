#define KRAKEN_EXPLICIT_THREAD_POOL_DEFINITION
#include "thread_pool.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void TaskQueue_init(TaskQueue* r)
{
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

static int ThreadPool_loop(void* in) {
    ThreadPool* const pool = (ThreadPool*)in;
    TaskQueue* const tasks = &(pool)->tasks;
    
    while (true) {
        
        while (TaskQueue_is_empty(tasks)) {
            if (pool->joined) {
                ++pool->ready_to_join;
                printf("Thread is joined.\n");
                goto exit;
            }
            
            printf("Thread is waiting for new tasks.\n");
            mtx_trylock(&pool->mtx);
            cnd_wait(&pool->cnd, &pool->mtx);
        }

        Task t = TaskQueue_pop(tasks);
        mtx_unlock(&pool->mtx);

        --pool->free_threads_count;

        printf("Task [%s, %ld] is processing.\n", t.name, (size_t)t.arg);
        t.f(t.arg);
        printf("Task [%s, %ld] is processed.\n", t.name, (size_t)t.arg);

        ++pool->free_threads_count;
    }

    exit:
}

ThreadPool *ThreadPool_create(size_t threads_count)
{
    ThreadPool* r = malloc(sizeof(ThreadPool));
    ThreadPool_init(r, threads_count);
    return r;
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

    p->free_threads_count = threads_count;

    for (size_t i = 0; i < threads_count; ++i) {
        thrd_create(&p->threads[i], &ThreadPool_loop, p);
    }
}

void ThreadPool_destroy(ThreadPool* const p) {
    assert(p != NULL);

    printf("Destroying ThreadPool. Remained tasks: %ld.\n", (size_t)p->tasks.len);

    TaskQueue_destroy(&p->tasks);
    free(p->threads);

    mtx_destroy(&p->mtx);
    cnd_destroy(&p->cnd);
}

void ThreadPool_plan(ThreadPool* const p, const Task task) {
    TaskQueue_push(&p->tasks, task);
    cnd_signal(&p->cnd);

    printf("Tasks [%s, %ld] is planned.\n", task.name, (size_t)task.arg);
}

void ThreadPool_join(ThreadPool* const p) {
    p->joined = true;

    while (p->ready_to_join != p->threads_len) {
        cnd_broadcast(&p->cnd);
    }

    for (size_t i = 0; i < p->threads_len; ++i) {
        thrd_join(p->threads[i], NULL);
    }
} 

size_t ThreadPool_freeThreadsCount(ThreadPool *const p) {
    assert(p != NULL);
    return p->free_threads_count;
}

size_t ThreadPool_tasksCount(ThreadPool*const p) {
    assert(p != NULL);
    return p->tasks.len;
}