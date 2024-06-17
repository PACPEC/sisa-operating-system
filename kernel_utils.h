
#ifndef __KERNEL_UTILS
#define __KERNEL_UTILS

extern void *user_stack;
extern void *system_stack;

extern unsigned int tics_timer;
extern unsigned int tecla_pulsada;

void RSG();
void umain();

// Copy byte to byte all the contents of different pointed buffers
void copy_to_user(void *sys, void *usr, int size);

#endif
