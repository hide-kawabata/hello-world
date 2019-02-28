        .text
        .globl _main
_main:
        pushq %rbp
        movq %rsp, %rbp

        leaq L1(%rip), %rdi
        movq $0, %rax
        callq printf

        movq $0, %rax
        leaveq
        retq

        .data
L1:     .string "Hello World!\n"
