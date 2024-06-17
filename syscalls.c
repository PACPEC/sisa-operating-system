#include "syscalls.h"
#include "kernel_utils.h"

// Syscalls code
// Get the global variable of number of ticks
void getTicks_call() {
    // no idea how to get the input parameters, why don't do it in assembly directly?
    // I'll get the input parameter of the user routine, in the userstack
    unsigned int *ret;
    __asm__ ("ld %0, 32(%1)" : "=r"(ret) : "r"(user_stack));
    copy_to_user(&tics_timer, ret, sizeof(unsigned int));
}

void getKey_call() {
    unsigned int *ret;
    __asm__ ("ld %0, 32(%1)" : "=r"(ret) : "r"(user_stack));
    copy_to_user(&tecla_pulsada, ret, sizeof(unsigned int));
}
