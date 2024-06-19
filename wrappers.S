.include "macros.s"

; No function stacks are created because all registers used are clobber ones

.globl getTicks
.type getTicks, @function
.align 0
getTicks:
    ld r1, 0(r7)
    movi r0, 0 ; syscall code 0 
    calls r0
    jmp r5
    
.globl getKey 
.type getKey, @function
.align 0
getKey:
    ld r1, 0(r7)
    movi r0, 1 ; syscall code 1
    calls r0
    jmp r5
