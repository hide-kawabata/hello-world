let prologue () =
  let code = "        .text\n"
           ^ "        .globl _main\n"
           ^ "_main:\n"
           ^ "        pushq %rbp\n"
           ^ "        movq %rsp, %rbp\n"
  in code

let epilogue () =
  let code = "        popq %rsi      # pop the resultant value\n"
           ^ "        leaq L1(%rip), %rdi\n"
           ^ "        movq $0, %rax\n"
           ^ "        callq _printf  # print the resultant value\n"
           ^ "        movq $0, %rax\n"
           ^ "        leaveq\n"
           ^ "        retq\n"
           ^ "        .data\n"
           ^ "L1:     .string \"%d\\n\"\n"
  in code


let codegen () =
  let str = read_line ()
  in let n = int_of_string str
  in let code1 = Printf.sprintf "        pushq $%d\n" n
  in let str = read_line ()
  in let n = int_of_string str
  in let code2 = Printf.sprintf "        pushq $%d\n" n
  in let code3 = "        popq %rbx\n"
               ^ "        popq %rax\n"
               ^ "        addq %rbx, %rax\n"
               ^ "        pushq %rax\n"
  in let code = prologue () ^ code1 ^ code2 ^ code3 ^ epilogue ()
  in Printf.printf "%s\n" code

let main () = codegen ()

let _ = main ()
