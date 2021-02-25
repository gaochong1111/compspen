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
(declare-fun y0 () Int)
(declare-fun x1 () Int)
(declare-fun y1 () Int)
(declare-fun z0_0 () Int)
(declare-fun sz0_0 () Int)
(declare-fun u0_0 () Int)
(declare-fun v0_0 () Int)

(assert
	(and
	(= x0 hst)
	(= y1 hen)
	(< x0 y1)
	(= y0 z0_0)
	(= (+ z0_0 sz0_0) x1)
	(tobool (ssep
		(hls x0 y0)
		(pto z0_0 (ref sz sz0_0))
		(hls x1 y1)
		)
	)
))
(assert (not
	(tobool (ssep
		(hls x0 y0)
		(hls y0 y1)
	)	)
))
;; end of problem
(check-sat)
;; 0.05s sat : the entl is false because Abs(A) \models Abs(B) is false
