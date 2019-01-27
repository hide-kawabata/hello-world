#An Extremely Small Compiler for the x86 Processor.


Usage

```
$ ./gen07 > tmp.s
(
+
4
3
)
$ cc tmp.s
$ ./a.out
7
```

```
$ cat sum.ms
(define (f x)
  (if (= x 1)
      x
      (+ (f (- x 1)) x)))

(f 10)
$ cat sum.ms | perl split.pl | ./gen10 > tmp.s
$ cc tmp.s
$ ./a.out
55
$
```

###Note

- Tested only on macOS.
- On Linux machines, replacing function names,
e.g., `_func` to `func`,
should make the programms runnable.


###Versions

gen01:
Adds two numbers. `1 2`

gen02:
A binary operation. `- 1 2`

gen03:
(modified)

gen04:
Sum of numbers. `1 2 3 4 5` 

gen05:
Supports Polish notation. `* - 3 4 5`

gen06:
Supports Polish notation as S-expressions. `(* (- 3 4) -5)`

gen07:
Optimized version of gen06.

gen08:
Supports if-expressions (true=1, false=0).
`(+ 10 (if (or (not (< 4 3)) (<= 2 5)) (+ 5 6) (- 7 8)))`

gen09:
Supports let-expressions.
`(let ((i 1) (j 2)) (+ i j))`

gen10:
Supports function definitions and function application.
`(define (f x y) (- x y)) (f 3 2)`

gen11:
Supports passing functions as arguments.
`(define (h f x) (f (g x)))
(define (g x) (- x 1))
(h g 3)`

`gen11` accepts a very small subset of the Scheme language:

- no anonymous function (i.e., no lambda)
- no closure
- no internal function definition
- no assignment
- no dynamic memory allocation
- no tail-call optimization
- naive one-pass code generataion
- fixed-sized stack frames
- supports only integer type (logical operators return 0 or 1)
- no syntax check
- no type check




#####TODO:

```
(if (not (not #t)) (* 5 6) (* 3 4))
(lambda (i j) (+ i j))
(define x (+ 1 2))
(define (f x y) (define (g y) (+ y 1)) (+ x (g y)))
```