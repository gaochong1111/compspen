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
				(and (>= (- ?y ?x) 3) (= ?f 0))
				(and (>= (- ?y ?x) 3) (= ?f 1))
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
(declare-fun u () Int)
(declare-fun x1 () Int)
(declare-fun x2 () Int)
(declare-fun y1 () Int)
(declare-fun y2 () Int)


(assert
	(and
		(= y1 x1)
		(> y2 y1)
		(tobool
			(ssep
				(pto x1 (ref sz u))
				(pto x2 (ref sz u))
				(hls y1 y2)
			)
		)
	)
)

(check-sat)

;;0.03s unsat
