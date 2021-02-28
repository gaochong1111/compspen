(set-logic QF_SLID_LC)
;; size field
(declare-fun sz () (Field Int Int))
(declare-fun st () (Field Int Int))

;; heap chunk
(define-fun hck ((?x Int) (?y Int) (?v Int)) Space
	(tospace
		(exists ((?f Int))
		(and
			(or
				(and (>= (- ?y ?x) 3) (= ?f 0))
				(and (>= (- ?y ?x) 3) (<= (- ?y ?x) ?v) (= ?f 1))
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
(define-fun hls ((?x Int) (?y Int) (?v Int)) Space
	(tospace (or
		(and (= ?x ?y) (tobool emp))
		(exists ((?w Int))
			(tobool	(ssep
					(hck ?x ?w ?v)
					(hls ?w ?y ?v)
					)
		))
	))
)

;; variables
(declare-fun x0 () Int)
(declare-fun sz0 () Int)
(declare-fun x1 () Int)

(declare-fun z0 () Int)
(declare-fun z1 () Int)
(declare-fun v () Int)

(assert
	(and	
		(= sz0 (- z1 x0))
		(= z0 x0)
		(tobool (ssep
			(pto x0 (ref sz sz0))
			(pto (+ x0 1) (ref st 1))
			(blk (+ x0 2) z1)
			)
		)
	)	
)

(assert
	(not
	(tobool 
		(hls z0 z1 5)
	)	
	)
)
;; end of problem
(check-sat)
;; 0.70s sat:the entl is false
