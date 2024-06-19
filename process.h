#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char pid; // PID = -1 is a free PCB
    void * sp;
    void * pc;
    void * kernel_sp;
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

#endif //PROCESS_H