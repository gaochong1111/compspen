(set-logic QF_SLID_LC)
;; size field
(declare-fun sz () (Field Int Int))

;; heap chunk
(define-fun hck ((?x Int) (?y Int)) Space
	(tospace
		(and
			(>= (- ?y ?x) 3)
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
(declare-fun x8 () Int)
(declare-fun sz8 () Int)
(declare-fun x9 () Int)
(declare-fun sz9 () Int)
(declare-fun xnxt () Int)
(declare-fun szall () Int)

(assert
	(and

	(= szall (+ sz0 sz1 sz2 sz3 sz4 sz5 sz6 sz7 sz8 sz9))
	(= hst x0)
	(= (+ x0 sz0) x1)
	(= (+ x1 sz1) x2)
	(= (+ x2 sz2) x3)
	(= (+ x3 sz3) x4)
	(= (+ x4 sz4) x5)
	(= (+ x5 sz5) x6)
	(= (+ x6 sz6) x7)
	(= (+ x7 sz7) x8)
	(= (+ x8 sz8) x9)
	(= (+ x9 sz9) xnxt)
	(tobool (ssep
		(pto x0 (ref sz szall))
		(blk (+ x0 1) x1)
		(hck x1 x2)
		(hck x2 x3)
		(hck x3 x4)
		(hck x4 x5)
		(hck x5 x6)
		(hck x6 x7)
		(hck x7 x8)
		(hck x8 x9)
		(hck x9 xnxt)
		(hls xnxt hen)
		)
	)
	)	)
;; end of problem
(check-sat)
;; 0.05s sat
