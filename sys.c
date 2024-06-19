#include "process.h"
#include "kernel_utils.h"

void sys_getTics() {
    int * tics_out = (int *) syscall_arg_1(current_task());
    copy_to_user(&tics_timer, tics_out, sizeof(unsigned int));
}

void sys_getKey() {
    int * key_out = (int *) syscall_arg_1(current_task());
    copy_to_user(&tecla_pulsada, key_out, sizeof(unsigned int));
}