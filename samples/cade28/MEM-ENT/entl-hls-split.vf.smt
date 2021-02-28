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

;; sz + MIN_SIZE <= szy &
;; sz >= MIN_SIZE &
;; nc = y + szy - sz &
;; z = y + szy
;; :
;; integer(y,szy-sz) *
;; blk(y+1,y+szy-sz) *
;; integer(nc, sz) *
;; blk(nc+1,z)
;; 
;; ==>
;; 
;; hck(y, szy-sz,nc) *
;; hck(nc, sz, z)

;; declare variables
(declare-fun sz () Int)
(declare-fun y  () Int)
(declare-fun szy () Int)
(declare-fun z  () Int)
(declare-fun nc () Int)

(assert (and
		(< 256 sz)
		(< 256 szy)
		(< (+ sz 256) szy)
		(= z (+ y szy))
		(= nc (+ y (- szy sz)))
		(tobool (ssep
			(pto y (ref f (- szy sz)))
			(blk (+ y 1) (+ y (- szy sz)))
			(pto nc (ref f sz))
			(blk (+ nc 1) z)
			)
		)
	)
)

(assert (not
		(tobool 
			(hls y z)
		)
	)
)

(check-sat)
;; 0.88s unsat : entl is true
