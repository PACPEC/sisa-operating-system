#include "kernel_utils.h"

void *user_stack = 0x4000;
void *system_stack = 0x8000;

// global kernel variables to update
unsigned int tics_timer = 0;
unsigned int tecla_pulsada = 0;

void copy_to_user(void *sys, void *usr, int size) {
    int i;
    for(i = 0; i < size; i++) {
        *(char *)usr = *(char *)sys;
    }
}

void timer_interrupt() {
    tics_timer++; // increment the number of ticks
    __asm__ ("out 6, %0" : : "a"(tics_timer)); // only for debugging purposes; it shows the value on the red LEDs
}
