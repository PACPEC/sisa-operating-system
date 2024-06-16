
#ifndef __KERNEL_UTILS
#define __KERNEL_UTILS

// global kernel variables to update
int tics_timer;
char tecla_pulsada;

void RSG();
void umain();

// Copy byte to byte all the contents of different pointed buffers
void copy_to_user(void *sys, void *usr, int size);

#endif
