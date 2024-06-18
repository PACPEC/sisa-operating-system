#include "process.h"

#include <stddef.h>

#define MAX_TASKS 2

extern char user_stack_bottom;

TaskUnion tasks[MAX_TASKS];
char next_free_pid = 0;

void init_tasks() {
    for (int i = 0; i < MAX_TASKS; ++i) {
        tasks[i].task.pid = -1;
    }
}

TaskStruct * create_task(void * main) {
    TaskUnion * tu = NULL;

    for (int i = 0; i < MAX_TASKS; ++i) {
        if (tasks[i].task.pid == -1) {
            tu = &tasks[i];
            break;
        }
    }

    if (tu) {
        // Assign a PID
        tu->task.pid = next_free_pid++;

        // Prepare for task switch
        tu->task.sp = &user_stack_bottom - (tu->task.pid * 512);
        tu->task.pc = main;
    }

    return &tu->task;
}

void userspace_jump(TaskStruct * task) {

    void * system_stack = ((TaskUnion *) task)->stack + sizeof(TaskUnion) - 2;

    __asm__(
        // Mask out system bit for reti PSW
        "rds r0, s7\n"
        "movi r1, -2\n" 
        "and r0, r0, r1\n"
        "wrs s0, r0\n"

        // Load the entry point to reti PC
        "wrs s1, %0\n"

        // Load the user stack to the SP
        "or r7, %1, %1\n"

        // Load the system stack to S6
        "wrs s6, %2\n"

        "reti"
        : : "r"(task->pc), "r"(task->sp), "r"(system_stack): "r0", "r1", "r7", "s0", "s1", "s6"
    );
}