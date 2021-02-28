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


;; (sz > 256 &
;; !(a = 0) &
;; !(sz = 0) &
;; !(sz = 1) :
;; pto(a, sz) *
;; blk(a+1,a+sz)
;; )
;; ==>
;; hls(a, a+sz)


;; declare variables
(declare-fun a () Int)
(declare-fun sz () Int)


(assert
	(and
		(< 256 sz)
		(distinct a 0)
		(distinct sz 0)
		(distinct sz 1)
		(tobool
			(ssep
				(pto a (ref f sz))
				(blk (+ a 1) (+ a sz))
			)
		)
	)
)

(assert
	(not
		(tobool
			(hls a (+ a sz))
		)
	)
)

(check-sat)
;;0.38s unsat:the entl is true
