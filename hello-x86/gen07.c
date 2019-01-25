#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char yytext[256];

#define I_MAX 256
int input_data[I_MAX];
int input_tok[I_MAX];
int count;
int eaten;

#define M_NUM 1
#define M_OP 2

//---------------------------------------------

void init_data() {
  count = 0;
  eaten = 1;
}

//---------------------------------------------

void read_sexpr();
void read_sexprlist();

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

void save_op(char sym[]) {
  input_tok[count] = M_OP;
  input_data[count] = sym[0];
  count++;
}

void save_num(int num) {
  input_tok[count] = M_NUM;
  input_data[count] = num;
  count++;
}

void save_token(char sym[]) {
  int num;
  if (sscanf(sym, "%d", &num) == 1) {
    save_num(num);
  } else if (strncmp(sym, "+", 1) == 0) { // +
    save_op(sym);
  } else if (strncmp(sym, "-", 1) == 0) { // -
    save_op(sym);
  } else if (strncmp(sym, "*", 1) == 0) { // *
    save_op(sym);
  } else if (strncmp(sym, "/", 1) == 0) { // /
    save_op(sym);
  }
}

void eat_token() {
  eaten = 1;
}

// ) | sym sexprlist
void read_sexprlist() {
  read_string();
  if (strncmp(yytext, ")", 1) == 0) { // )
    eat_token();
  } else { // sym sexprlist
    read_sexpr();
    read_sexprlist();
  }
}

// sym | ( sexprlist
void read_sexpr() {
  read_string();
  if (strncmp(yytext, "(", 1) == 0) { // (
    eat_token();
    read_sexprlist();
  } else { // sym
    eat_token();
    save_token(yytext);
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

void binop(char op) {
  printf("        popq %%rax\n");
  printf("        popq %%rbx\n");
  if (op == '+') {
    printf("        addq %%rbx, %%rax\n");
  } else if (op == '-') {
    printf("        subq %%rbx, %%rax\n");
  } else if (op == '*') {
    printf("        imulq %%rbx, %%rax\n");
  } else if (op == '/') {
    printf("        cqto\n");
    printf("        idivq %%rbx\n");
  } else {
    exit(1);
  }
  printf("        pushq %%rax\n");
}

void codegen() {
  prologue();
  for (int i = count - 1; i >= 0; i--) {
    if (input_tok[i] == M_NUM) {
      printf("        pushq $%d\n", input_data[i]);
    } else if (input_tok[i] == M_OP) {
      binop(input_data[i]);
    }
  }
  epilogue();
}


//---------------------------------------------

int main() {
  init_data();
  read_sexpr();
  codegen();
  return 0;
}
