#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char pid; // PID = -1 is a free PCB
    void * sp;
    void * pc;
} TaskStruct;

typedef union
{
    TaskStruct task;
    char stack[512];
} TaskUnion;

void init_tasks();

void prepare_userspace_jump(TaskStruct * task);

TaskStruct * create_task(void * main);

#endif //PROCESS_H