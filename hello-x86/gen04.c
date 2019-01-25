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

void binop(char op) {
  printf("        popq %%rax\n");
  printf("        popq %%rbx\n");
  if (op == '+') {
    printf("        addq %%rax, %%rbx\n");
  } else if (op == '-') {
    printf("        subq %%rax, %%rbx\n");
  } else {
    exit(1);
  }
  printf("        pushq %%rbx\n");
}

void codegen(int n[], int c) {
  int i;
  prologue();
  printf("        pushq $%d\n", n[c-1]);
  for (i = c - 2; i >= 0; i--) {
    printf("        pushq $%d\n", n[i]);
    binop('+');
  }
  epilogue();
}

int main() {
  int n;
  int count = 0;
  int nums[256];
  char buf[32];
  while (fgets(buf, 32, stdin)) {
    sscanf(buf, "%d", &n);
    nums[count] = n;
    count++;
  }
  codegen(nums, count);
  return 0;
}
