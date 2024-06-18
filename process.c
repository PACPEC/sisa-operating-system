#include "process.h"

#include <stddef.h>

#define MAX_TASKS 2

extern char user_stack_bottom;
extern void * user_stack;

extern void __finRSG();

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

        // Prepare for userspace jump
        tu->task.sp = &user_stack_bottom - (tu->task.pid * 512);
        tu->task.pc = main;

        // Prepare for task switch
        tu->stack[sizeof(tu->stack) - 11] = __finRSG;
        tu->task.kernel_sp = &tu->stack[sizeof(tu->stack) - 14];
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

void task_switch(TaskStruct * task) {
    task->sp = user_stack;

    __asm__(
        "rds r0, s5\n"
        "st 0(%0), r0\n"
        "st 0(%1), r7\n"
        "ld r7, 0(%2)\n"
        "ld r0, 0(%3)\n"
        "wrs s5, r0"
        : : "r"(current_task()->pc), "r"(&current_task()->kernel_sp), "r"(&task->kernel_sp), "r"(&task->pc)
    );

    user_stack = task->sp;

    void * system_stack = ((TaskUnion *) task)->stack + sizeof(TaskUnion) - 2;
    __asm__("wrs s6, %0" : : "r"(system_stack));
}

TaskStruct * current_task() {
    void * sp;
    __asm__("st 0(%0), r7" : : "r"(&sp));

    for (int i = 0; i < MAX_TASKS; ++i) {
        if (&tasks[i] <= sp && sp < &tasks[i+1]) {
            return &tasks[i];
        }
    }

    return NULL;
}