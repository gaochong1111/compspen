(set-logic QF_SLID_LC)
;; size field
(declare-fun f () (Field Int Int))

;; heap chunk
(define-fun hck ((?x Int) (?y Int)) Space
	(tospace
		(and
			(>= (- ?y ?x) 2)
			(tobool	(ssep
					(pto ?x (ref f (- ?y ?x)))
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

;; hst = x0 &
;; 256 <= sz0 &
;; x1 < hen &
;; x1 = hst + sz0 &
;; 256 <= sz1 &
;; xnxt = x1 + sz1 
;; :
;; integer(hst,sz0+sz1) *
;; blk(hst+1,x1) *
;; integer(x1, sz1) *
;; blk(x1+1,xnxt) *
;; hls(xnxt, hen)
;; 
;; ==>
;; 
;; hck(hst,sz0+sz1,xnxt) *
;; hls(xnxt,hen)

;; declare variables
(declare-fun hst () Int)
(declare-fun hen () Int)

(declare-fun x0 () Int)
(declare-fun sz0 () Int)
(declare-fun x1 () Int)
(declare-fun sz1 () Int)
(declare-fun xnxt () Int)
(declare-fun szall () Int)

(assert (and
		(< 256 sz0)
		(= hst x0)
		(= x1 (+ x0 sz0))
		(= xnxt (+ x1 sz1))
		(= szall (+ sz0 sz1))
		(tobool (ssep
			(pto x0 (ref f sz0))
			(blk (+ x0 1) x1)
			(pto x1 (ref f sz1))
			(blk (+ x1 1) xnxt)
			(hls xnxt hen)
			)
		)
	)
)

(assert (not
		(tobool (ssep
			(pto hst (ref f szall))
			(blk (+ hst 1) xnxt)
			(hls xnxt hen)
			)
		)
	)
)

(check-sat)
;; 0.75s sat : entl is false because sz0 != szall
