.include "macros.s"

; data section
; all the vectors of RSI and RSE will be saved here
.data
    .balign 2 ; if access, shouldn't be necessary 
    interrupts_vector:
        .word RSI__interrup_timer     ; 0 Interval Timer
        .word RSI_default_resume      ; 1 Pulsadores (KEY)
        .word RSI_default_resume      ; 2 Interruptores (SWITCH)
        .word RSI__interrup_keyboard  ; 3 Teclado PS/2

    exceptions_vector:
        .word RSE_default_halt    ;  0 Instruccion ilegal
        .word RSE_default_halt    ;  1 Acceso a memoria no alineado
        .word RSE_default_resume  ;  2 Overflow en coma flotante
        .word RSE_default_resume  ;  3 Division por cero en coma flotante
        .word RSE_default_halt    ;  4 Division por cero
        .word RSE_default_halt    ;  5 No definida
        .word RSE_excepcion_TLB   ;  6 Miss en TLB de instrucciones
        .word RSE_excepcion_TLB   ;  7 Miss en TLB de datos
        .word RSE_excepcion_TLB   ;  8 Pagina invalida al TLB de instrucciones
        .word RSE_excepcion_TLB   ;  9 Pagina invalida al TLB de datos
        .word RSE_default_halt    ; 10 Pagina protegida al TLB de instrucciones
        .word RSE_default_halt    ; 11 Pagina protegida al TLB de datos
        .word RSE_default_halt    ; 12 Pagina de solo lectura
        .word RSE_default_halt    ; 13 Excepcion de proteccion
        
    call_sys_vector:
        .word getTics_call  ; 0 Hay que definirla en el S.O.
        .word getKey_call   ; 1 Hay que definirla en el S.O.
        .word RSE_default_resume  ; 2 Hay que definirla en el S.O.
        .word RSE_default_resume  ; 3 Hay que definirla en el S.O.
        .word RSE_default_resume  ; 4 Hay que definirla en el S.O.
        .word RSE_default_resume  ; 5 Hay que definirla en el S.O.
        .word RSE_default_resume  ; 6 Hay que definirla en el S.O.
        .word RSE_default_resume  ; 7 Hay que definirla en el S.O.
    
; code section 
.text
    ; ============================================
    ;                initialization
    ; ============================================
    entry_point:
        ; Define the stack
        $MOVEI r7, system_stack_bottom
        ; Define the return pointer
        $MOVEI r5, hlt
        ; Jump to C
        $MOVEI r6, kmain
        jmp r6

    hlt:
        halt

    ; ============================================
    ;           default service routines 
    ; ============================================
    RSI_default_resume: JMP R6
    RSE_default_halt:   HALT
    RSE_default_resume: JMP R6

    RSE_excepcion_TLB:   ; code fragment
        ; code of the to do task still missing
        rds   R2, S1         ;you have to execute once again the instruction that failed
        addi  R2, R2, -2
        wrs   S1, R2
        JMP R6

    ; ============================================
    ;        Interrupt service routine
    ;    (only entry point to the system)
    ; ============================================
    .global RSG
    .global __finRSG ; for first task switch
    RSG: ; Salvar el estado
         $push  R0, R1, R2, R3, R4, R5, R6
         $MOVEI r0, user_stack
         st     0(r0), r7         ; save the user stack point to global kernel variable
         $MOVEI r0, system_stack
         ld     r7, 0(r0)
         ; rds    R7, S6
         rds    R1, S0
         rds    R2, S1
         rds    R3, S3
         $push  R1, R2, R3
         rds    R1, S2                    ;check S2 contents
         movi   R2, 14
         cmpeq  R3, R1, R2                ;if equals 14 will be a syscall 
         bnz    R3, __call_sistema        ;jump to the syscalls if S2==14
         movi   R2, 15
         cmpeq  R3, R1, R2                ;if queals 15 will be an interrupt
         bnz    R3, __interrupcion        ;jump to the interrupts if S2==15
    __excepcion:
         movi   R2, lo(exceptions_vector)
         movhi  R2, hi(exceptions_vector)
         add    R1, R1, R1                ;R1 contains the exception indentifier
         add    R2, R2, R1
         ld     R2, 0(R2)
         jal    R6, R2
         bz     R3, __finRSG
    __call_sistema:
         rds    R1, S3                    ;S3 contains the syscall identifier
         movi   R2,7
         and    r1,r1,r2                  ;we are left with lower 3bits, limit the number of services given by the OS
         add    R1, R1, R1
         movi   R2, lo(call_sys_vector)
         movhi  R2, hi(call_sys_vector)
         add    R2, R2, R1
         ld     R2, 0(R2)
         jal    R6, R2
         bnz    R3, __finRSG
    __interrupcion:
         getiid R1
         add    R1, R1, R1
         movi   R2, lo(interrupts_vector)
         movhi  R2, hi(interrupts_vector)
         add    R2, R2, R1
         ld     R2, 0(R2)
         jal    R6, R2
    __finRSG: ;Restaurar el estado
         $pop  R3, R2, R1
         wrs   S3, R3
         wrs   S1, R2
         wrs   S0, R1
         $MOVEI r0, user_stack
         ld    r7, 0(r0)        ; restore the user stack context upon returning
         $pop  R6, R5, R4, R3, R2, R1, R0
         reti

    ; ============================================
    ;           Interrupt service routine 
    ; ============================================
    RSI__interrup_timer:
        $MOVEI r1, timer_interrupt 
        jal    r5, r1
        ; all this will be treated onto C code timer_interrupt() routine
        ; $MOVEI r4, tics_timer     ;loads the memory address pointing the data with the number of clock ticks that have arrived
        ; ld     r3, 0(r4)          ;loads the number of ticks
        ; addi   r3, r3, 1          ;increments them
        ; st     0(r4), r3          ;updates the variable with the number of ticks
        ; out 6,r3                  ;only for DEBUG; it shows the value on the red LEDs
        jmp    r6                 ;R6 holds the return address (as it has not been modified) to end the RSG (end of interrupt service)

    ; ============================================
    ;           Interrupt service routine 
    ; ============================================
    RSI__interrup_keyboard:
        in     r3, 15             ;read the value with the ASCII char corresponding the pressed key
        $MOVEI r4, tecla_pulsada  ;loads the memory address where to store the result of the pressed key 
        st     0(r4), r3          ;updates the variable with the new pressed key
        jmp    r6                 ;R6 contains (as it has not been modified) the return address to end the RSG (end of interrupt service)

    ; ============================================
    ;               Syscalls code 
    ; ============================================
    getTics_call:
        $push   r3
        $MOVEI  r4, tics_timer ; load the tics_timer variable
        ld      r3, 0(r4)
        $MOVEI  r0, user_stack
        ld      r0, 0(r0)
        ld      r0, 14(r0)
        st      0(r0), r3 ; return to user_stack the value of tics_timer
        $pop    r3
        jmp     r6

    getKey_call:
        $push   r3
        $MOVEI  r4, tecla_pulsada ; load the tics_timer variable
        ld      r3, 0(r4)
        $MOVEI  r0, user_stack
        ld      r0, 0(r0) 
        ld      r0, 14(r0) ; pointer to user stack
        st      0(r0), r3 ; return to user_stack the value of tics_timer
        $pop    r3
        jmp     r6

