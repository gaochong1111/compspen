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
(declare-fun xnxt () Int)
(declare-fun szall () Int)

(assert
	(and

	(= szall (+ sz0 sz1))
	(= hst x0)
	(= (+ x0 sz0) x1)
	(= (+ x1 sz1) xnxt)
	(tobool (ssep
		(pto x0 (ref sz szall))
		(blk (+ x0 1) x1)
		(pto x1 (ref sz sz1))
		(blk (+ x1 1) xnxt)
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
;; 0.81s unsat : the entl is true
