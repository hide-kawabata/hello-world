#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//---------------------------------------------

int label_count;
int newlabel() {
  int l = label_count;
  label_count++;
  return l;
}

//---------------------------------------------

char yytext[256];
int eaten;

void init_data() {
  label_count = 0;
  eaten = 1;
}

//---------------------------------------------

void read_C();
void read_E();

int read_string() {
  char buf[32];
  char str[16];
  int len;
  if (eaten > 0) {
    fgets(buf, 32, stdin);
    sscanf(buf, "%s", str);
    len = strlen(str);
    strncpy(yytext, str, len);
    yytext[len] = '\0';
    eaten = 0;
  } else {
    len = strlen(yytext);
  }
  return len;
}


void eat_token() {
  eaten = 1;
}

void read_C() {
  read_string();
  if (strncmp(yytext, "+", 1) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rbx\n");
    printf("        popq %%rax\n");
    printf("        addq %%rbx, %%rax\n");
    printf("        pushq %%rax\n");
  } else if (strncmp(yytext, "-", 1) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rbx\n");
    printf("        popq %%rax\n");
    printf("        subq %%rbx, %%rax\n");
    printf("        pushq %%rax\n");
  } else if (strncmp(yytext, "*", 1) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rbx\n");
    printf("        popq %%rax\n");
    printf("        imulq %%rbx, %%rax\n");
    printf("        pushq %%rax\n");
  } else if (strncmp(yytext, "/", 1) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rbx\n");
    printf("        popq %%rax\n");
    printf("        cqto\n");
    printf("        idivq %%rbx\n");
    printf("        pushq %%rax\n");
  } else if (strncmp(yytext, "if", 2) == 0) {
    eat_token();
    read_E();
    printf("        popq %%rax\n");
    printf("        cmpq $0, %%rax\n");
    int labF = newlabel();
    printf("        je F%d\n", labF);
    read_E();
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    read_E();
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, ">=", 2) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rax\n");
    printf("        popq %%rbx\n");
    printf("        cmpq %%rax, %%rbx\n");
    int labF = newlabel();
    printf("        jge F%d\n", labF);
    printf("        pushq $0\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $1\n");
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, ">", 1) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rax\n");
    printf("        popq %%rbx\n");
    printf("        cmpq %%rax, %%rbx\n");
    int labF = newlabel();
    printf("        jg F%d\n", labF);
    printf("        pushq $0\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $1\n");
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, "<=", 2) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rax\n");
    printf("        popq %%rbx\n");
    printf("        cmpq %%rax, %%rbx\n");
    int labF = newlabel();
    printf("        jle F%d\n", labF);
    printf("        pushq $0\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $1\n");
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, "<", 1) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rax\n");
    printf("        popq %%rbx\n");
    printf("        cmpq %%rax, %%rbx\n");
    int labF = newlabel();
    printf("        jl F%d\n", labF);
    printf("        pushq $0\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $1\n");
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, "=", 1) == 0) {
    eat_token();
    read_E();
    read_E();
    printf("        popq %%rbx\n");
    printf("        popq %%rax\n");
    printf("        cmpq %%rax, %%rbx\n");
    int labF = newlabel();
    printf("        je F%d\n", labF);
    printf("        pushq $0\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $1\n");
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, "and", 3) == 0) {
    eat_token();
    read_E();
    printf("        popq %%rax\n");
    printf("        cmpq $0, %%rax\n");
    int labF = newlabel();
    printf("        je F%d\n", labF);
    read_E();
    printf("        popq %%rax\n");
    printf("        cmpq $0, %%rax\n");
    printf("        je F%d\n", labF);
    printf("        pushq $1\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $0\n");
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, "or", 2) == 0) {
    eat_token();
    read_E();
    printf("        popq %%rax\n");
    printf("        cmpq $0, %%rax\n");
    int labF = newlabel();
    printf("        jne F%d\n", labF);
    read_E();
    printf("        popq %%rax\n");
    printf("        cmpq $0, %%rax\n");
    printf("        jne F%d\n", labF);
    printf("        pushq $0\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $1\n");
    printf("E%d:\n", labE);
  }
}

// E ::= num | ( C )
void read_E() {
  read_string();
  if (strncmp(yytext, "(", 1) == 0) {
    eat_token();
    read_C();
    read_string(); // must be ")"
    eat_token();
  } else { // num
    eat_token();
    printf("        pushq $%d\n", atoi(yytext));
  }
}

//---------------------------------------------

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


//---------------------------------------------

int main() {
  init_data();
  prologue();
  read_E();
  epilogue();
  return 0;
}
