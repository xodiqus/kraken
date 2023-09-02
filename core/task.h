#ifndef KRAKEN_TASK_INCLUDED
#define KRAKEN_TASK_INCLUDED

#include <threads.h>
#include "fundamental_types.h"
#include "kdebug.h"


typedef enum TaskStatus {
    TS_fail,
    TS_success,
    TS_cancel,
} TaskStatus;

// typedef enum TaskType {
//     TT_single               = 1,
//     TT_group_one_important  = 2,
//     TT_group_all_important  = 4,
// } TaskType;

#ifdef KRAKEN_IMPLICIT_DEFINITION

struct TaskControlBlock;
struct Task;
struct TaskGroup;

typedef TaskStatus (*PTask)(const void* in, void* out, TaskControlBlock*);

#else 

#include <stdatomic.h>

typedef struct TaskControlBlock {
    atomic_bool    ready;
    atomic_bool    cancel;
    cnd_t*         cnd;
} TaskControlBlock;

typedef TaskStatus (*PTask)(const void* in, void* out, TaskControlBlock*);

typedef struct Task {
    // TaskStatus (*f)(const void* in, void* out, TaskControlBlock*);
    PTask f;

    const void* in;
          void* out;

    TaskControlBlock control;

    kdebug_code(const char* name);
} Task;

typedef struct TaskGroup {
    cnd_t cnd;
    Task* tasks;
    umax  tasks_len;
    umax  tasks_cap;
} TaskGroup;

#endif


void TCB_make_ready(TaskControlBlock*);
bool TCB_cancelled(TaskControlBlock*);

// Allocate new name with format for Task and return it.
const char* Task_name(Task*, const char* format, ...);

TaskGroup* TaskGroup_new(umax reserve);
void       TaskGroup_delete(TaskGroup*);
void TaskGroup_init(TaskGroup*, umax reserve);
void TaskGroup_destroy(TaskGroup*);

Task* TaskGroup_emplace(TaskGroup*, PTask, const void* in, void* out, const char* name);

void TaskGroup_await_one(TaskGroup*);
void TaskGroup_await_all(TaskGroup*);
void TaskGroup_cancel(TaskGroup*);

#endif