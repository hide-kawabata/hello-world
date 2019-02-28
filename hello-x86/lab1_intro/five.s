        .text
        .globl _main
_main:
        pushq %rbp
        movq %rsp, %rbp

        movq $5, %rsi
        leaq L1(%rip), %rdi
        movq $0, %rax
        callq _printf

        movq $0, %rax
        leaveq
        retq

        .data
L1:     .string "%d\n"
