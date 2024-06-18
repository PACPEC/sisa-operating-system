#include "kernel_utils.h"
#include "process.h"

extern void corre_letras_main();

__attribute__((noreturn)) void kmain() {

    init_tasks();
    TaskStruct * ts = create_task(corre_letras_main);
    
    __asm__ (
      "wrs s5, %0\n\t"
      
      "movi r0, lo(system_stack)\n\t"
      "movhi r0, hi(system_stack)\n\t"
      "st  0(r0), r7\n\t" // the system stack, in theory can always be the same from this point on

      : // no output operands
      : "r"(RSG)
      : "r0", "s5" /*acknowledge battered registers to compiler*/);
    
  userspace_jump(ts);

  for (;;); // infinte loop - Instead of halting
}
