entry_point:
        ; Define the stack
        movi r7, lo(system_stack_bottom)
        movhi r7, hi(system_stack_bottom)

        ; Define the return pointer
        movi r5, lo(hlt)
        movhi r5, hi(hlt)

        ; Jump to C
        movi r6, lo(kmain)
        movhi r6, hi(kmain)
        jmp r6

hlt:
        halt
