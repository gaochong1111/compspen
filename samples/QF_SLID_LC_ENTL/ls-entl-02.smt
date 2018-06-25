
(set-logic QF_SLID_LC)

;; declare sorts
(declare-sort Sll_t 0)


;; declare fields
(declare-fun next () (Field Sll_t Sll_t))
(declare-fun data () (Field Sll_t Int))

;; declare predicates

(define-fun sls 
	((?in Sll_t) (?dt1 Int)  (?out Sll_t) (?dt2 Int)) 
	Space 
	(tospace 
		(or 
			(and (= ?in ?out) (= ?dt1 ?dt2)
			(tobool emp)
			)

 
			(exists 
				((?u Sll_t) (?dt3 Int)) 
				(and  	
					(> ?dt1 (+ ?dt3 1))
					(tobool (ssep 
						(pto ?in (sref (ref next ?u) (ref data ?dt1)) ) 
						(sls ?u ?dt3 ?out ?dt2)
						) 
					)
				)
			)
		)
	)
)

;; declare variables
(declare-fun X () Sll_t)
(declare-fun Y () Sll_t)
(declare-fun Z () Sll_t)

(declare-fun a () Int)
(declare-fun b0 () Int)
(declare-fun b1 () Int)
(declare-fun c () Int)


;; formula
(assert
	(and
		(> a b0)
		(> b1 c)

		(tobool
			(ssep
				(sls X a Y b0 )
                (sls Y b1 Z c)
			)
		)
	)
)

(assert
	(not
	(and
		(> a (+ c 3))
		(tobool
			(sls X a Z c)
	)
	)
)
)


(check-sat)

;; sat: entl is false
