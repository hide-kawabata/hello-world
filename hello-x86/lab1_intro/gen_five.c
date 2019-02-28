#include <stdio.h>

void codegen() {
    printf("        .text\n");
    printf("        .globl _main\n");
    printf("_main:\n");
    printf("        pushq %%rbp\n");
    printf("        movq %%rsp, %%rbp\n");
    printf("\n");
    printf("        movq $5, %%rsi\n");
    printf("        leaq L1(%%rip), %%rdi\n");
    printf("        movq $0, %%rax\n");
    printf("        callq _printf\n");
    printf("\n");
    printf("        movq $0, %%rax\n");
    printf("        leaveq\n");
    printf("        retq\n");
    printf("\n");
    printf("        .data\n");
    printf("L1:     .string \"%%d\\n\"\n");
}

int main() {
  codegen();
  return 0;
}
