#include <stdio.h>

void codegen(int n) {
    printf("        .text\n");
    printf("        .globl _main\n");
    printf("_main:\n");
    printf("        pushq %%rbp\n");
    printf("        movq %%rsp, %%rbp\n");
    printf("\n");
    printf("        movq $%d, %%rsi\n", n);
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
  int n;
  char buf[32];
  fgets(buf, 32, stdin);
  sscanf(buf, "%d", &n);
  codegen(n);
  return 0;
}
