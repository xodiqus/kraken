#include "task.h"

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "kdebug.h"

const char* Task_name(Task* t, const char *format, ...) {
    assert(format != NULL);

#ifdef KRAKEN_DEBUG

    char* const name = malloc(100);
    assert(name != NULL);

    va_list args;
    va_start(args, format);

    vsprintf(name, format, args);

    va_end(args);

    if (t != NULL) {
        t->name = name;
    }

    return name;
#else 
    return NULL;
#endif
}

TaskGroup* TaskGroup_new(umax reserve) {
    TaskGroup* g = malloc(sizeof(TaskGroup));
    assert(g != NULL);

    TaskGroup_init(g, reserve);
    return g;
}

void TaskGroup_delete(TaskGroup* g) {
    assert(g != NULL);

    TaskGroup_destroy(g);
    free(g);
}

void Task_init(Task* t, PTask f, const void* in, void* out) {
    assert(t != NULL);

    t->f = f; 
    t->in = in; 
    t->out = out; 

    t->control.cancel = false;
    t->control.ready = false;
    t->control.cnd = NULL;

    kdebug_code(
        t->name = NULL
    );
}

void Task_destroy(Task* t) {
    assert(t != NULL);

    kdebug_code(
        free((char*)t->name);
        memset(t, 0, sizeof(Task));
    );
}

void TaskGroup_init(TaskGroup *g, umax reserve) {
    assert(g != NULL);

    cnd_init(&g->cnd);
    g->tasks = malloc(reserve * sizeof(Task));
    g->tasks_cap = reserve;
    g->tasks_len = 0;
}

void TaskGroup_destroy(TaskGroup *g) {
    assert(g != NULL);

    cnd_destroy(&g->cnd);

    kdebug_code(
        for (umax i = 0; i < g->tasks_len; ++i) {
            Task_destroy(&g->tasks[i]);
        }
    )

    free(g->tasks);

    kdebug_code(
        memset(g, 0, sizeof(TaskGroup));
    );
}

Task* TaskGroup_emplace(TaskGroup* g, PTask f, const void* in, void* out, const char* name) {
    assert(g != NULL); 
    assert(g->tasks_len < g->tasks_cap);

    Task *const t = &g->tasks[g->tasks_len];
    Task_init(t, f, in, out);
    Task_name(t, name);

    t->control.cnd = &g->cnd;
    
    ++g->tasks_len;
    return t;
}

void TaskGroup_await_one(TaskGroup *g)
{
    assert(g != NULL);

    kdebug_log("await one of %lu tasks", g->tasks_len);

    mtx_t mtx; 
    mtx_init(&mtx, mtx_plain);
    mtx_lock(&mtx);
    
    bool ready = false;

    // while (!ready) {
        for (umax i = 0; i < g->tasks_len && !ready; i++) {
            ready = ready || (bool)g->tasks[i].control.ready;
        }

        cnd_wait(&g->cnd, &mtx);
    // }

    mtx_unlock(&mtx);
    mtx_destroy(&mtx);
}

void TaskGroup_await_all(TaskGroup* g) {
    assert(false && "Implemetation is unable!");
}

void TaskGroup_cancel(TaskGroup* g) {
    assert(g != NULL);

    kdebug_code(umax cancelled = 0);

    for (umax i = 0; i < g->tasks_len; ++i) {
        g->tasks[i].control.cancel = true;

        kdebug_code(
            if (!g->tasks[i].control.ready) {
                ++cancelled;
            }
        )

        kdebug_log("cancel [%s]", g->tasks[i].name);
    }

    kdebug_log("cancel totally %lu tasks", cancelled);
}

void TCB_make_ready(TaskControlBlock* b) {
    assert(b != NULL);

    b->ready = true;
    cnd_signal(b->cnd);
}

bool TCB_cancelled(TaskControlBlock* b) {
    assert(b != NULL);
    // kdebug_log("check either cancel or not [%d]", b->cancel);
    return b->cancel;
}
