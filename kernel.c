void RSG();
void umain();

__attribute__((noreturn)) void kmain() {
  // User space switch
  int *userpc = umain;
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
