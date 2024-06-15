
// global kernel variables to update
int tics_timer;
char tecla_pulsada;

void RSG();
void umain();

// Auxiliary functions code

// Copy byte to byte all the contents of different pointed buffers
void copy_to_user(void *sys, void *usr, int size) {
    int i;
    for(i = 0; i < size; i++) {
        *(char *)usr = *(char *)sys;
    }
}

__attribute__((noreturn)) void kmain() {
  // User space switch
  int *userpc = umain;
  int a = *(int *)0x9000;
  __asm__ (
    "wrs s0, %0\n\t"
    "wrs s1, %1\n\t"
    "wrs s2, %2\n\t"
    "wrs s3, %3\n\t"
    "wrs s5, %4\n\t"
    "reti"
    : // no output operands
    : "r"(0x0002 /*interrupts enable, user mode*/), 
      "r"(userpc), 
      "r"(14 /*syscall interrupt code*/), 
      "r"(0 /*syscall code, 0 by default*/),
      "r"(RSG)
    : "s0", "s1", "s2", "s3", "s5" /*acknowledge battered registers to compiler*/);

  for (;;); // infinte loop - Instead of halting
}

// Syscalls code
// could be put onto another file

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
