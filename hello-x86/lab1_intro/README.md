# Lab1

## Aim

- get used to C language
- get used to x86 assembly language

## Tasks

### T1

Build an x86 code generator that

- reads two integers from stdio, and 
- emits an x86 assembly program that computes the sum of them.

```
$ ./lab1_prog > tmp.c
3
4
$ cc tmp.c
$ ./a.out
7
$
```



### T2

Make it accept an arbitrary number of integers from stdio.

```
$ ./lab1_prog > tmp.c
4  <-- the number of integers that are to be given
1
2
3
4
$ cc tmp.c
$ ./a.out
10
```


### T3

Make it accept a sequence of integers from stdio.

```
$ ./lab1_prog > tmp.c
3
4
5
6 
(Ctrl-D)
$ cc tmp.c
$ ./a.out
18
```

### T4

Make it possible to instruct operations, e.g., support computations such as 3 + 4 - 5 or 3 * 4 - 5.
You can treat an expression 3 + 4 * 5 as (3+4)*5.

```
$ ./lab1_prog > tmp.c
3
+
4
-
5
(Ctrl-D)
$ cc tmp.c
$ ./a.out
2
```

### T5

Make it possible to calculate values of expressions properly; i.e., "3 + 4 * 5" should be 3 + (4 * 5).
Parentheses should be acceptable; i.e., "(3 + 4) * 5".

```
$ ./lab1_prog > tmp.c
(
3
+
4
)
*
5
(Ctrl-D)
$ cc tmp.c
$ ./a.out
35
```


### Others

You are encurraged to apply other extentions.



## Example

Following programs are given:

- a code that compute 3 + 4
- a code that compute two given numbers

