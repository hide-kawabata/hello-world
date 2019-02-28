#include <stdio.h>
#include <stdlib.h>

void prologue() {
  printf("        .text\n");
  printf("        .globl _main\n");
  printf("_main:\n");
  printf("        pushq %%rbp\n");
  printf("        movq %%rsp, %%rbp\n");
}

void epilogue() {
  printf("        popq %%rsi\n");
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
  int n;
  int count = 0;
  char buf[32];
  prologue();
  while (fgets(buf, 32, stdin)) {
    sscanf(buf, "%d", &n);
    printf("        pushq $%d\n", n);
    count++;
  }
  for (int i = 0; i < count - 1; i++) {
    printf("        popq %%rbx\n");
    printf("        popq %%rax\n");
    printf("        addq %%rbx, %%rax\n");
    printf("        pushq %%rax\n");
  }
  epilogue();
  return 0;
}
