Extremely small compiler for the x86 processor.

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
$ echo '(+ (/ 4 2) (* 3 -1))' | perl split.pl | ./gen07 > tmp.s
$ cc tmp.s
$ ./a.out
-1
$
```

Versions

gen01:
Generate asm programs that compute the sum of given two numbers.

gen02:
- 1 2

gen03:
(modified)

gen04:
Sum of numbers.

gen05:
* - 3 4 5

gen06:
(* (- 3 4) -5)

gen07:
Optimized version of gen06.


TODO:

```
(if (not (not t)) (* 5 6) (* 3 4))
(lambda (i j) (+ i j))
(let ((i 1) (j 2)) (+ i j))
(define x (+ 1 2))
(define (f x y) (define (g y) (+ y 1)) (+ x (g y)))
```