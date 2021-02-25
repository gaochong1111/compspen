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



;; 256 <= oldsz &
;; y = hst + oldsz &
;; t = hst + oldsz
;; :
;; integer(hst,oldsz) *
;; blk(hst+1,y) *
;; hls(y,hen)
;; 
;; ==>
;; 
;; hck(hst, oldsz, t) *
;; hls(t, hen)

;; declare variables
(declare-fun hst () Int)
(declare-fun hen () Int)

(declare-fun x0 () Int)
(declare-fun sz0 () Int)
(declare-fun y () Int)
(declare-fun t () Int)

(assert (and
		(< 256 sz0)
		(= hst x0)
		(= y (+ x0 sz0))
		(= t (+ x0 sz0))
		(tobool (ssep
			(pto x0 (ref f sz0))
			(blk (+ x0 1) y)
			(hls y hen)
			)
		)
	)
)

(assert (not
		(tobool (ssep
			(pto hst (ref f sz0))
			(blk (+ hst 1) t)
			(hls y hen)
			)
		)
	)
)

(check-sat)
;; 0.78s unsat : entl is true
