(define (f x)
  (if (= x 1)
      x
      (+ (f (- x 1)) x)))

(f 1000)
