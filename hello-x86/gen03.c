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

void codegen(char op, int a, int b) {
  prologue();
  printf("        pushq $%d\n", a);
  printf("        pushq $%d\n", b);
  binop(op);
  epilogue();
}

int main() {
  int n1, n2;
  char op;
  char buf[32];
  fgets(buf, 32, stdin);
  sscanf(buf, "%c", &op);
  fgets(buf, 32, stdin);
  sscanf(buf, "%d", &n1);
  fgets(buf, 32, stdin);
  sscanf(buf, "%d", &n2);
  codegen(op, n1, n2);
  return 0;
}
