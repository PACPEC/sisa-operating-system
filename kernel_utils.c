#include "kernel_utils.h"

void copy_to_user(void *sys, void *usr, int size) {
    int i;
    for(i = 0; i < size; i++) {
        *(char *)usr = *(char *)sys;
    }
}
