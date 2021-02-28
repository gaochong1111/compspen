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

;; 256 <= szy &
;; z = (y + szy) &
;; 256 <= szz &
;; u = z + szz 
;; :
;; integer(y,szy+szz) *
;; blk(y+1,z) *
;; integer(z,szz) *
;; blk(z+1,u)
;; 
;; ==>
;; 
;; hck(y, szy+szz, u)

;; declare variables
(declare-fun y () Int)
(declare-fun szy () Int)
(declare-fun z () Int)
(declare-fun szz () Int)
(declare-fun u () Int)

(assert (and
		(< 256 szy)
		(= z (+ y szy))
		(< 256 szz)
		(= u (+ z szz))
		(tobool (ssep
			(pto y (ref f (+ szy szz)))
			(blk (+ y 1) z)
			(pto z (ref f szz))
			(blk (+ z 1) u)
			)
		)
	)
)

(assert (not
		(tobool (ssep
			(pto y (ref f (+ szy szz)))
			(blk (+ y 1) u)
			)
		)
	)
)

(check-sat)
;; 0.32s unsat : entl is true
