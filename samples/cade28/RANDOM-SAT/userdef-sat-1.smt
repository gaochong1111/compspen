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
				(and (= ?f 0) (>= (- ?y ?x) 3))
				(and (= ?f 1) (>= (- ?y ?x) 3) (<= (- ?y ?x) ?v))
			)
			(tobool	(ssep
					(pto ?x (ref sz (- ?y ?x)))
					(pto (+ ?x 1) (ref st (- ?f)))
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

(declare-fun y0 () Int)
(declare-fun y1 () Int)

(declare-fun z0 () Int)
(declare-fun z1 () Int)
(declare-fun v () Int)

(assert
	(and
	(= v 5)
	(tobool (ssep
		(pto x0 (ref sz sz0))
		(blk (+ x0 1) x1)
		(hck y0 y1 v)
		(hls z0 z1 v)
		)
	)
	)	)
;; end of problem
(check-sat)
;; 0.03s sat
