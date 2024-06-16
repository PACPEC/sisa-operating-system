#include "syscalls.h"
#include "kernel_utils.h"

// Syscalls code
// Get the global variable of number of ticks
void getTicks_call() {
    // no idea how to get the input parameters, why don't do it in assembly directly?
    // I'll get the input parameter of the user routine, in the userstack
    int *ret;
    __asm__ ("ld %0, 48(r7)" : "=r"(ret) : );
    copy_to_user(&tics_timer, ret, sizeof(int));
}

void getKey_call() {
    char *ret;
    __asm__ ("ld %0, 48(r7)" : "=r"(ret) : );
    copy_to_user(&tecla_pulsada, ret, sizeof(char));
}
