#include "kernel_utils.h"

__attribute__((noreturn)) void kmain() {
    // User space switch
    int * userpc = (int *)umain;
    
    __asm__ (
      "wrs s0, %0\n\t" // modifiy the system registers for the user "return"
      "wrs s1, %1\n\t"
      "wrs s2, %2\n\t"
      "wrs s3, %3\n\t"
      "wrs s5, %4\n\t"
      
      "movi r0, lo(system_stack)\n\t"
      "movhi r0, hi(system_stack)\n\t"
      "st  0(r0), r7\n\t" // the system stack, in theory can always be the same from this point on
      "movi r7, lo(user_stack_bottom)\n\t"
      "movhi r7, hi(user_stack_bottom)\n\t"
      
      "reti" // return al context d'usuari
      : // no output operands
      : "r"(0x0002 /*interrupts enable, user mode*/), 
        "r"(userpc), 
        "r"(14 /*syscall interrupt code*/), 
        "r"(0 /*syscall code, 0 by default*/),
        "r"(RSG)
      : "s0", "s1", "s2", "s3", "s5" /*acknowledge battered registers to compiler*/);

  for (;;); // infinte loop - Instead of halting
}
