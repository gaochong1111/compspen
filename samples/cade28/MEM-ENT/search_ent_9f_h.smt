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
(declare-fun rsz () Int)
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

(assert
	(and
	(distinct hst hen)
	(= hst x0)
	(= (+ x0 sz0) x1)
	(< sz0 rsz)
	(= (+ x1 sz1) x2)
	(< sz1 rsz)
	(= (+ x2 sz2) x3)
	(< sz2 rsz)
	(= (+ x3 sz3) x4)
	(< sz3 rsz)
	(= (+ x4 sz4) x5)
	(< sz4 rsz)
	(= (+ x5 sz5) x6)
	(< sz5 rsz)
	(= (+ x6 sz6) x7)
	(< sz6 rsz)
	(= (+ x7 sz7) x8)
	(< sz7 rsz)
	(= (+ x8 sz8) x9)
	(< sz8 rsz)
	(= (+ x9 sz9) xnxt)
	(>= sz9 rsz)
	(tobool (ssep
		(hck x0 x1)
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
(assert (not
	(tobool (ssep
		(hls hst xnxt)
		(hls xnxt hen)
	)	)
))
;; end of problem
(check-sat)
;; 7.02s unsat : the entl is true
