#include "kernel_utils.h"
#include "process.h"

extern void corre_letras_main();
extern void fibonacci_main();

__attribute__((noreturn)) void kmain() {

    init_tasks();
    TaskStruct * ts1 = create_task(corre_letras_main);
    TaskStruct * ts2 = create_task(fibonacci_main);
    
    __asm__ ("wrs s5, %0" : : "r"(RSG) : "s5");
    
  //task_switch(ts1); // This should override the userspace jump
                    // (just for testing, current_task is not
                    // properly set and a second task switch will fail)

  userspace_jump(ts1);

  for (;;); // infinte loop - Instead of halting
}
