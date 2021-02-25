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
(declare-fun x1 () Int)
(declare-fun x2 () Int)
(declare-fun y1 () Int)
(declare-fun y2 () Int)
(declare-fun z1 () Int)
(declare-fun z2 () Int)

(assert
	(and
		(< t1 x1)
		(< t1 y1)
		(< z1 (- z2 3))
		(> z1 0)
		(> z2 0)
		(tobool
			(ssep
				(pto t1 (ref sz 3))
				(blk x1 x2)
				(hls y1 y2)
			)
		)
	)
)

(assert
	(not
		(and
			(< z1 z2)
			(tobool
				(hls z1 z2)
			)
		)
	)
)

(check-sat)
;;3.72s sat:the entl is false
