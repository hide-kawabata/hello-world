#include <stdio.h>

void codegen() {
  printf("        .text\n");
  printf("        .globl _main\n");
  printf("_main:\n");
  printf("        pushq %%rbp\n");
  printf("        movq %%rsp, %%rbp\n");
  printf("        pushq $1\n");
  printf("        pushq $2\n");
  printf("        popq %%rax\n");
  printf("        popq %%rbx\n");
  printf("        addq %%rax, %%rbx\n");
  printf("        pushq %%rbx\n");
  printf("        popq %%rax\n");
  printf("        movq %%rax, %%rsi\n");
  printf("        leaq L1(%%rip), %%rdi\n");
  printf("        movq $1, %%rax\n");
  printf("        callq _printf\n");
  printf("        movq $0, %%rax\n");
  printf("        movq %%rbp, %%rsp\n");
  printf("        popq %%rbp\n");
  printf("        retq\n");
  printf("        .data\n");
  printf("L1:     .string \"%%d\\n\"\n");
}

int main() {
  codegen();
  return 0;
}
