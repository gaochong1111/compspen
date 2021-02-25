(set-logic QF_SLID_LC)
;; size field
(declare-fun sz () (Field Int Int))
(declare-fun st () (Field Int Int))

;; heap chunk
(define-fun hck ((?x Int) (?b1 Int) (?y Int) (?b2 Int) (?v Int)) Space
	(tospace
		(and
			(or
				(and (>= (- ?y ?x) 4) (<= (- ?y ?x) ?v) (= ?b1 0) (= ?b2 1))
				(and (>= (- ?y ?x) 4) (<= (- ?y ?x) ?v) (= ?b1 1) (= ?b2 0))
			)
			(tobool	(ssep
					(pto ?x (ref sz (- ?y ?x)))
					(pto (+ ?x 1) (ref st ?b2))
					(blk (+ ?x 2) ?y)
				)
			)
		)
	)
)
;; heap list
(define-fun hls ((?x Int) (?b1 Int) (?y Int) (?b2 Int) (?v Int)) Space
	(tospace (or
		(and (= ?x ?y) (= ?b1 ?b2) (tobool emp))
		(exists ((?w Int) (?st Int))
			(tobool	(ssep
					(hck ?x ?b1 ?w ?st ?v)
					(hls ?w ?st ?y ?b2 ?v)
					)
		))
	))
)

;; variables
(declare-fun x0 () Int)
(declare-fun sz0 () Int)
(declare-fun x1 () Int)

(declare-fun x2 () Int)
(declare-fun x3 () Int)

(declare-fun x4 () Int)
(declare-fun x5 () Int)

(declare-fun b1 () Int)
(declare-fun b2 () Int)
(declare-fun b3 () Int)
(declare-fun b4 () Int)

(declare-fun v () Int)

(assert
	(and
	(= x1 x2)
	(= b3 0)
	(= b4 0)
	(= v 5)
	(= (- x5 x4) 10)
	(tobool (ssep
		(pto x0 (ref sz sz0))
		(blk (+ x0 1) x1)
		(hck x2 b1 x3 b2 v)
		(hls x4 b3 x5 b4 v)
		)
	)
	)	)
;; end of problem
(check-sat)
;; 0.03s sat x5-x4 should be [8,10] [16,20] [24,30] [32,40] [40,50] ...
