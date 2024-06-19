#include "process.h"
#include "kernel_utils.h"

#include <stddef.h>

#define MAX_TASKS 2

extern char user_stack_bottom;
extern void * user_stack;

extern void __finRSG();

TaskUnion tasks[MAX_TASKS] __attribute__ ((section (".kernel_tasks")));
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

        // Treat as if started with the system
        tu->task.resume_tick = 0;

        // Prepare for userspace jump
        tu->task.sp = &user_stack_bottom - (tu->task.pid * 512);
        tu->task.pc = main;

        // Prepare for task switch
        tu->stack[sizeof(tu->stack)/sizeof(int) -  9] = tu->task.sp;
        tu->stack[sizeof(tu->stack)/sizeof(int) - 10] = 0x2; // PSW with user mode and interrupts
        tu->stack[sizeof(tu->stack)/sizeof(int) - 11] = tu->task.pc;
        tu->stack[sizeof(tu->stack)/sizeof(int) - 13] = __finRSG;
        tu->task.kernel_sp = &tu->stack[sizeof(tu->stack)/sizeof(int) - 15];
    }

    return &tu->task;
}

void userspace_jump(TaskStruct * task) {
    TaskUnion * tu = (TaskUnion *) task;

    void * system_stack = &tu->stack[sizeof(tu->stack)/sizeof(int) - 1];

    __asm__(
        // Mask out system bit and enable interrupts for reti PSW
        "rds r0, s7\n"
        "movi r1, -2\n" 
        "and r0, r0, r1\n"
        "movi r1, 2\n"
        "or r0, r0, r1\n"
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
    __asm__(
        "st 0(%0), r7\n"
        "ld r7, 0(%1)\n"
        : : "r"(&current_task()->kernel_sp), "r"(&task->kernel_sp)
    );
}

TaskStruct * current_task() {
    void * sp;
    __asm__("st 0(%0), r7" : : "r"(&sp));

    // Dirty hack because the C compiler isn't behaving :(
    TaskUnion * tasks = 0x9000;

    for (int i = 0; i < MAX_TASKS; ++i) {
        if (&tasks[i] <= sp && sp < &tasks[i+1]) {
            return &tasks[i];
        }
    }

    return NULL;
}

int syscall_arg_1(TaskStruct * task) {
    TaskUnion * tu = (TaskUnion *) task;

    return tu->stack[sizeof(tu->stack)/sizeof(int) - 3];
}

TaskStruct * next_sched_task = NULL;

void sched_ready(TaskStruct * task) {
    next_sched_task = task;
}

void sched_update() {
    // NO OP
}

int sched_should_switch() {
    const unsigned int QUANTUM = 10;

    if (tics_timer - current_task()->resume_tick >= QUANTUM) {
        return next_sched_task != NULL;
    } else {
        return 0;
    }
}

void sched_next() {
    TaskStruct * next = next_sched_task;
    next_sched_task = current_task();
    
    task_switch(next);

    current_task()->resume_tick = tics_timer;
}