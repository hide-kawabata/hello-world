#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//---------------------------------------------

#define NAME_LENGTH_MAX 16
#define VAR_MAX 256

struct VarInfo_t {
  char name[NAME_LENGTH_MAX];
};
struct IdTable_t {
  struct VarInfo_t var[VAR_MAX];
  int n_vars;
};

struct IdTable_t idtable;

int label_count;
int newlabel() {
  int l = label_count;
  label_count++;
  return l;
}

int lookup_id(char name[]) {
  int i;
  for (i = idtable.n_vars-1; i >= 0; i--) {
    if (strcmp(idtable.var[i].name, name) == 0) break;
  }
  return i * 8 + 8;
}

int register_id(char name[]) {
  int n = idtable.n_vars;
  if (n >= VAR_MAX) return -2; // too many ids
  strcpy(idtable.var[n].name, name);
  idtable.n_vars++;
  return n * 8 + 8;
}
    

//---------------------------------------------

char yytext[256];
int eaten;

void init_data() {
  label_count = 0;
  eaten = 1;
  idtable.n_vars = 0;
}

//---------------------------------------------

void read_C();
void read_E();
void read_B();
void read_B2();

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

// B2 ::= eps | B
void read_B2() {
  read_string();
  if (strncmp(yytext, ")", 1) == 0) { // eps
    // do nothing
  } else {
    read_B();
  }
}

// B ::= ( id E ) B2
void read_B() {
  read_string(); // must be "("
  eat_token();
  read_string(); // must be an id
  char name[16];
  strcpy(name, yytext);
  eat_token();
  int addr = register_id(yytext);
  read_E();
  printf("        popq %%rax\n");
  printf("        movq %%rax, -%d(%%rbp)  # var %s\n",
	 addr, name);
  read_string(); // must be ")"
  eat_token();
  read_B2();
}

// C ::= op E E | if E E E | id E | let ( B ) E
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
  } else if (strncmp(yytext, "not", 3) == 0) {
    eat_token();
    read_E();
    printf("        popq %%rax\n");
    printf("        cmpq $0, %%rax\n");
    int labF = newlabel();
    printf("        je F%d\n", labF);
    printf("        pushq $0\n");
    int labE = newlabel();
    printf("        jmp E%d\n", labE);
    printf("F%d:\n", labF);
    printf("        pushq $1\n");
    printf("E%d:\n", labE);
  } else if (strncmp(yytext, "let", 3) == 0) {
    eat_token();
    read_string(); // must be "("
    eat_token();
    int cur = idtable.n_vars;
    read_B();
    read_string(); // must be ")"
    eat_token();
    read_E();
    idtable.n_vars = cur;
  }
}

// E ::= num | ( C ) | id
void read_E() {
  read_string();
  if (strncmp(yytext, "(", 1) == 0) {
    eat_token();
    read_C();
    read_string(); // must be ")"
    eat_token();
  } else if (isdigit(yytext[0])) {
    eat_token();
    printf("        pushq $%d\n", atoi(yytext));
  } else { // id
    eat_token();
    int addr  = lookup_id(yytext);
    printf("        movq -%d(%%rbp), %%rax    # var %s\n",
	   addr, yytext);
    printf("        pushq %%rax\n");
  }
}

//---------------------------------------------

void prologue() {
  printf("        .text\n");
  printf("        .globl _main\n");
  printf("_main:\n");
  printf("        pushq %%rbp           # save frame ptr\n");
  printf("        movq %%rsp, %%rbp      # update frame ptr\n");
  printf("        addq $-128, %%rsp     # fixed frame size\n");
}

void epilogue() {
  printf("        popq %%rax\n");
  printf("        movq %%rax, %%rsi\n");
  printf("        leaq L1(%%rip), %%rdi\n");
  printf("        movq $1, %%rax\n");
  printf("        callq _printf\n");
  printf("        movq $0, %%rax\n");
  printf("        movq %%rbp, %%rsp      # restore stack ptr\n");
  printf("        popq %%rbp            # restore frame ptr\n");
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
