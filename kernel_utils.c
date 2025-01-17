#include "kernel_utils.h"
#include "process.h"

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
    __asm__ ("out 6, %0" : : "r"(tics_timer)); // only for debugging purposes; it shows the value on the red LEDs

    sched_update();
    if (sched_should_switch()) {
        sched_next();
    }
}
