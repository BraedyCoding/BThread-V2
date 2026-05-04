#include "context_abi.inc"

.global context_switch
.type context_switch, @function

context_switch:

    movq %rbx, 0(%rdi)
    movq %rbp, 8(%rdi)
    movq %r12, 16(%rdi)
    movq %r13, 24(%rdi)
    movq %r14, 32(%rdi)
    movq %r15, 40(%rdi)
    movq %rsp, 48(%rdi)

    leaq resume_point(%rip), %rax
    movq %rax, 56(%rdi)

    movq 0(%rsi), %rbx
    movq 8(%rsi), %rbp
    movq 16(%rsi), %r12
    movq 24(%rsi), %r13
    movq 32(%rsi), %r14
    movq 40(%rsi), %r15
    movq 48(%rsi), %rsp
    movq 64(%rsi), %rdi

    jmp *56(%rsi)

resume_point:
    ret