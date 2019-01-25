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
Add two  numbers. `1 2`

gen02:
A binary operation. `- 1 2`

gen03:
(modified)

gen04:
Sum of numbers. `1 2 3 4 5` 

gen05:
Support Polish notation. `* - 3 4 5`

gen06:
Support Polish notation as S-expressions. `(* (- 3 4) -5)`

gen07:
Optimized version of gen06.

gen08:
Support if-expressions (true=1, false=0).
`(+ 10 (if (or (< 4 3) (<= 2 5)) (+ 5 6) (- 7 8)))`


TODO:

```
(if (not (not t)) (* 5 6) (* 3 4))
(lambda (i j) (+ i j))
(let ((i 1) (j 2)) (+ i j))
(define x (+ 1 2))
(define (f x y) (define (g y) (+ y 1)) (+ x (g y)))
```