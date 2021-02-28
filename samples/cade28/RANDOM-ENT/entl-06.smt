(set-logic QF_SLID_LC)
;; size field
(declare-fun sz () (Field Int Int))
(declare-fun st () (Field Int Int))

;; heap chunk
(define-fun hck ((?x Int) (?y Int)) Space
	(tospace
		(exists ((?f Int))
		(and
			(or
				(and (>= (- ?y ?x) 4) (= ?f 0))
				(and (>= (- ?y ?x) 4) (= ?f 1))
			)
			(tobool	(ssep
					(pto ?x (ref sz (- ?y ?x)))
					(pto (+ ?x 1) (ref st ?f))
					(blk (+ ?x 2) ?y)
				)
			)
		)
		)
	)
)
;; heap list
(define-fun hls ((?x Int) (?y Int)) Space
	(tospace (or
		(and (= ?x ?y) (tobool emp))
		(exists ((?w Int))
			(tobool	(ssep
					(hck ?x ?w)
					(hls ?w ?y)
					)
		))
	))
)



;; declare variables
(declare-fun t1 () Int)
(declare-fun t2 () Int)
(declare-fun x1 () Int)
(declare-fun x2 () Int)


(assert
	(and
		(< t1 t2)
		(tobool
			(ssep
				(pto t1 (ref sz 10))
				(pto t2 (ref sz 11))
			)
		)
	)
)

(assert
	(not
		(and
			(< x1 x2)
			(tobool
				(blk x1 x2)
			)
		)
	)
)

(check-sat)
;;0.04s sat:the entl is false because abs(A) entl abs(B) is false
