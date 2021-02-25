(set-logic QF_SLID_LC)
;; size field
(declare-fun sz () (Field Int Int))

;; heap chunk
(define-fun hck ((?x Int) (?y Int)) Space
	(tospace
		(and
			(>= (- ?y ?x) 2)
			(tobool	(ssep
					(pto ?x (ref sz (- ?y ?x)))
					(blk (+ ?x 1) ?y)
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

;; variables
(declare-fun hst () Int)
(declare-fun hen () Int)
(declare-fun x0 () Int)
(declare-fun sz0 () Int)
(declare-fun x1 () Int)
(declare-fun sz1 () Int)
(declare-fun x2 () Int)
(declare-fun sz2 () Int)
(declare-fun x3 () Int)
(declare-fun sz3 () Int)
(declare-fun x4 () Int)
(declare-fun sz4 () Int)
(declare-fun x5 () Int)
(declare-fun sz5 () Int)
(declare-fun x6 () Int)
(declare-fun sz6 () Int)
(declare-fun x7 () Int)
(declare-fun sz7 () Int)
(declare-fun xnxt () Int)
(declare-fun szall () Int)

(assert
	(and

	(= szall (+ sz0 sz1 sz2 sz3 sz4 sz5 sz6 sz7))
	(= hst x0)
	(= (+ x0 sz0) x1)
	(= (+ x1 sz1) x2)
	(= (+ x2 sz2) x3)
	(= (+ x3 sz3) x4)
	(= (+ x4 sz4) x5)
	(= (+ x5 sz5) x6)
	(= (+ x6 sz6) x7)
	(= (+ x7 sz7) xnxt)
	(tobool (ssep
		(pto x0 (ref sz szall))
		(blk (+ x0 1) x1)
		(pto x1 (ref sz sz1))
		(blk (+ x1 1) x2)
		(pto x2 (ref sz sz2))
		(blk (+ x2 1) x3)
		(pto x3 (ref sz sz3))
		(blk (+ x3 1) x4)
		(pto x4 (ref sz sz4))
		(blk (+ x4 1) x5)
		(pto x5 (ref sz sz5))
		(blk (+ x5 1) x6)
		(pto x6 (ref sz sz6))
		(blk (+ x6 1) x7)
		(pto x7 (ref sz sz7))
		(blk (+ x7 1) xnxt)
		(hls xnxt hen)
		)
	)
	)	)
(assert (not
	(tobool (ssep
		(pto hst (ref sz szall))
		(blk (+ hst 1) xnxt)
		(hls xnxt hen)
	)	)
))
;; end of problem
(check-sat)
;; 3.30s unsat : the entl is true
