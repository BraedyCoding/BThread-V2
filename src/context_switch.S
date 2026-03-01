;************************************************************
; context_switch.S
;
; Context switching routine for Bthread (user-level threads)
; Saves callee-saved registers from old_ctx and restores
; registers from new_ctx, then jumps to the new thread's RIP.
;************************************************************

#include "commonMacros.hpp"   ; contains 'offset' for 32/64-bit
; Do NOT include Context.hpp because structs can't be used in asm

.global context_switch
.type context_switch, @function

;------------------------------------------------------------------------------
; void context_switch(Context* old_ctx, Context* new_ctx)
; Arguments:
;   RDI -> pointer to old_ctx (first argument)
;   RSI -> pointer to new_ctx (second argument)
;------------------------------------------------------------------------------

context_switch:

    ;--------------------------------------------
    ; Save current thread's registers into old_ctx
    ;--------------------------------------------
    mov [rdi + 0*offset], rbx    ; save rbx
    mov [rdi + 1*offset], rbp    ; save rbp
    mov [rdi + 2*offset], r12    ; save r12
    mov [rdi + 3*offset], r13    ; save r13
    mov [rdi + 4*offset], r14    ; save r14
    mov [rdi + 5*offset], r15    ; save r15
    mov [rdi + 6*offset], rsp    ; save stack pointer

    ;--------------------------------------------
    ; Load new thread's registers from new_ctx
    ;--------------------------------------------
    mov rbx, [rsi + 0*offset]   ; restore rbx
    mov rbp, [rsi + 1*offset]   ; restore rbp
    mov r12, [rsi + 2*offset]   ; restore r12
    mov r13, [rsi + 3*offset]   ; restore r13
    mov r14, [rsi + 4*offset]   ; restore r14
    mov r15, [rsi + 5*offset]   ; restore r15
    mov rsp, [rsi + 6*offset]   ; restore stack pointer

    ;--------------------------------------------
    ; Jump to new thread's instruction pointer
    ;--------------------------------------------
    jmp [rsi + 7*offset]         ; rip is last in struct

    ; function never returns
