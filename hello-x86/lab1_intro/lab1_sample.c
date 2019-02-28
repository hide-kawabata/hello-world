#include <stdio.h>

void prologue() {
  printf("        .text\n");
  printf("        .globl _main\n");
  printf("_main:\n");
  printf("        pushq %%rbp\n");
  printf("        movq %%rsp, %%rbp\n");
  printf("#------------------------\n");
}

void epilogue() {
  printf("#------------------------\n");
  printf("        popq %%rsi      # pop the resultant value\n");
  printf("        leaq L1(%%rip), %%rdi\n");
  printf("        movq $0, %%rax\n");
  printf("        callq _printf  # print the resultant value\n");
  printf("        movq $0, %%rax\n");
  printf("        leaveq\n");
  printf("        retq\n");
  printf("        .data\n");
  printf("L1:     .string \"%%d\\n\"\n");
}

int codegen() {
  int n;
  char buf[32];
  prologue();

  fgets(buf, 32, stdin);
  sscanf(buf, "%d", &n);
  printf("        pushq $%d\n", n);

  fgets(buf, 32, stdin);
  sscanf(buf, "%d", &n);
  printf("        pushq $%d\n", n);
  printf("#------------------------\n");
  printf("        popq %%rbx\n");
  printf("        popq %%rax\n");
  printf("        addq %%rbx, %%rax\n");
  printf("        pushq %%rax\n");

  epilogue();
  return 0;
}

int main() {
  codegen();
  return 0;
}
