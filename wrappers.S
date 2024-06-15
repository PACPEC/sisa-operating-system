.include "macros.s"

.globl getTicks
.type getTicks, @function
.align 0
getTicks:
    addi r7,r7, -2
    st 0(r7), r0 ; save the r0 reg to the user stack 
    movi r0, 0 ; syscall code 0 
    calls r0
    ld r0, 0(r7)
    addi r7,r7, 2
    jmp r6 
    
