#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char pid; // PID = -1 is a free PCB
    void * sp;
    void * pc;
    void * kernel_sp;
    unsigned int resume_tick;
} TaskStruct;

typedef union
{
    TaskStruct task;
    int stack[256];
} TaskUnion;

void init_tasks();

TaskStruct * create_task(void * main);

void userspace_jump(TaskStruct * task);

void task_switch(TaskStruct * task);

TaskStruct * current_task();

int syscall_arg_1(TaskStruct * task);

void sched_ready(TaskStruct * task);

void sched_update();

int sched_should_switch();

void sched_next();

#endif //PROCESS_H