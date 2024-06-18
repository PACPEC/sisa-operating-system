.include "macros.s"

.globl getTicks
.type getTicks, @function
.align 0
getTicks:
    addi r7,r7, -2
    st 0(r7), r0 ; save the r0 reg to the user stack 
    ld r1, 2(r7)
    $push r1
    movi r0, 0 ; syscall code 0 
    calls r0
    ld r0, 0(r7)
    addi r7,r7, 2
    jmp r5 
    
.globl getKey 
.type getKey, @function
.align 0
getKey:
    addi r7,r7, -2
    st 0(r7), r0 ; save the r0 reg to the user stack 
    ld r1, 2(r7)
    $push r1
    movi r0, 1 ; syscall code 1
    calls r0
    ld r0, 0(r7)
    addi r7,r7, 2
    jmp r5 
