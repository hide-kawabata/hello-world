#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//---------------------------------------------

#define R_ARG 0
#define R_LV 1
#define R_FUN 2

#define NAME_LENGTH_MAX 16
#define VAR_MAX 16

struct VarInfo_t {
  char name[NAME_LENGTH_MAX];
  int ty;
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
  int count_arg = 1;
  for (i = idtable.n_vars-1; i >= 0; i--) {
    if (idtable.var[i].ty == R_ARG) count_arg++;
    if (strcmp(idtable.var[i].name, name) == 0) break;
  }
  if (i >= 0 && idtable.var[i].ty == R_ARG) {
//    return (i * 8 + 16);
//    printf("(lookup_id) name=%s, arg\n", name);
    return (count_arg * 8);
  } else if (i >= 0 && idtable.var[i].ty == R_LV) {
//    printf("(lookup_id) name=%s, local\n", name);
    return -(i * 8 + 8);
  } else if (i >= 0) {
//    printf("(lookup_id) name=%s, global\n", name);
    return 0;
  } else {
//    printf("(lookup_id) name=%s, not found\n", name);
    return 0;
  }
}


int register_id(char name[], int ty) {
  int n = idtable.n_vars;
  if (n >= VAR_MAX) return -2; // too many ids
  strcpy(idtable.var[n].name, name);
  idtable.var[n].ty = ty;
  idtable.n_vars++;
  if (ty == R_ARG) {
    return (n * 8 + 8);
  } else if (ty == R_LV) {
    return -(n * 8 + 8);
  } else {
    return 0;
  }
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
void read_E2();
int read_Es();
void read_IDs();
void prologue(char []);
void epilogue();
void main_epilogue();
void main_data();

int read_string() {
  char buf[32];
  char str[16];
  int len;
  if (eaten > 0) {
    if (fgets(buf, 32, stdin)) {
      sscanf(buf, "%s", str);
      len = strlen(str);
      strncpy(yytext, str, len);
      yytext[len] = '\0';
      eaten = 0;
    } else { // EOF
      len = 0;
    }
  } else {
    len = strlen(yytext);
  }
  return len;
}


void eat_token() {
  eaten = 1;
}

// IDs ::= eps | id IDs
void read_IDs() {
  read_string();
  if (strncmp(yytext, ")", 1) == 0) { // eps
    // do nothing
  } else {
    char name[16];
    strcpy(name, yytext);
    eat_token();
    register_id(name, R_ARG); // pre-order
    read_IDs();
//    register_id(name, R_ARG); // post-order
  }
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
  int addr = register_id(yytext, R_LV);
  read_E();
  printf("        popq %%rax\n");
  printf("        movq %%rax, %d(%%rbp)  # var %s\n",
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
  } else if (strncmp(yytext, "define", 6) == 0) {
    eat_token();
    read_string(); // must be "("
    eat_token();
    read_string(); // func name
    eat_token();
    char fname[16];
    sprintf(fname, "_%s", yytext);
    register_id(fname, R_FUN);
    int cur = idtable.n_vars;
    read_IDs(); // parameters
    read_string(); // must be ")"
    eat_token();
    prologue(fname);
    printf("# function body\n");
    read_E(); // func body
    printf("# end of function body\n");
    idtable.n_vars = cur;
  } else { // ID : function application
    eat_token();

    char fname[16];
//    sprintf(fname, "_%s", yytext);
    strcpy(fname, yytext);
    int nargs = read_Es(); // arguments

    int addr = lookup_id(fname);
    if (addr > 0) { // R_ARG
      printf("        movq %d(%%rbp), %%rbx # function\n", addr);
      printf("        callq *%%rbx\n");
    } else if (addr == 0) { // R_FUN
      printf("        callq _%s\n", fname);
    }
    for (int i = 0; i < nargs; i++) {
      printf("        popq %%rbx      # dummy\n");
    }
    printf("        pushq %%rax      # returned value\n");
  }
}

// Es ::= eps | E Es
int read_Es() {
  read_string();
  if (strncmp(yytext, ")", 1) == 0) {
    // do nothing
    return 0;
  } else {
    read_E();
    return 1 + read_Es();
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
  } else { // id (could be a function)
    eat_token();
    int addr = lookup_id(yytext);
    if (addr == 0) { // function
      char buf[16];
      sprintf(buf, "_%s", yytext);
      printf("        leaq %s(%%rip), %%rax\n", buf);
      printf("        pushq %%rax\n");
    } else { // var
      printf("        movq %d(%%rbp), %%rax    # var %s\n",
	     addr, yytext);
      printf("        pushq %%rax\n");
    }
  }
}

// E' ::= num | ( C ) | id
void read_E2() {
  read_string();
  if (strncmp(yytext, "(", 1) == 0) {
    eat_token();
    read_string();
    int mainflag = 0;
    if (strncmp(yytext, "define", 6) != 0) {
      mainflag = 1;
    }
    if (mainflag) prologue("_main");
    read_C();
    if (mainflag) main_epilogue(); else epilogue();
    read_string(); // must be ")"
    eat_token();
  } else if (isdigit(yytext[0])) {
    eat_token();
    prologue("_main");
    printf("        pushq $%d\n", atoi(yytext));
    main_epilogue();
  } else { // id
    eat_token();
    prologue("_main");
    int addr  = lookup_id(yytext);
    printf("        movq %d(%%rbp), %%rax    # var %s\n",
	   addr, yytext);
    printf("        pushq %%rax\n");
    main_epilogue();
  }
}

// Es ::= eps | E' Es
void read_Es2() {
  int len = read_string();
  if (len > 0) {
    read_E2();
    read_Es2();
  }
}

// P ::= E' Es
void read_P() {
  read_E2();
  read_Es2();
}

//---------------------------------------------

void prologue(char name[]) {
  printf("        .globl %s\n", name);
  printf("%s:\n", name);
  printf("        pushq %%rbp           # save frame ptr\n");
  printf("        movq %%rsp, %%rbp      # update frame ptr\n");
  printf("        addq $-128, %%rsp     # fixed frame size\n");
}

void main_epilogue() {
  printf("        popq %%rax\n");
  printf("        movq %%rax, %%rsi\n");
  printf("        leaq L1(%%rip), %%rdi\n");
  printf("        movq $1, %%rax\n");
  printf("        callq _printf\n");
  printf("        movq $0, %%rax\n");
  printf("        movq %%rbp, %%rsp      # restore stack ptr\n");
  printf("        popq %%rbp            # restore frame ptr\n");
  printf("        retq\n");
}

void epilogue() {
  printf("        popq %%rax\n");
  printf("        movq %%rbp, %%rsp      # restore stack ptr\n");
  printf("        popq %%rbp            # restore frame ptr\n");
  printf("        retq\n");
}

void main_text() {
  printf("        .text\n");
}

void main_data() {
  printf("        .data\n");
  printf("L1:     .string \"%%d\\n\"\n");
}


//---------------------------------------------

int main() {
  init_data();
  main_text();
  read_P();
  main_data();
  return 0;
}
