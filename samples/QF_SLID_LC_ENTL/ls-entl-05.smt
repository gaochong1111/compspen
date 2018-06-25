
(set-logic QF_SLID_LC)

;; declare sorts
(declare-sort Sll_t 0)

;; declare fields
(declare-fun next () (Field Sll_t Sll_t))

;; declare predicates
(define-fun lls 
	((?in Sll_t) (?len1 Int) (?out Sll_t) (?len2 Int)) 
	Space 
	(tospace 
		(or 
			(and (= ?in ?out) (= ?len1 ?len2) 
			(tobool emp)
			)

 
			(exists 
				((?u Sll_t) (?len3 Int)) 
				(and  	
					(= ?len1 (+ ?len3 1))
					(tobool (ssep 
						(pto ?in (ref next ?u) ) 
						(lls ?u ?len3 ?out ?len2)
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
(declare-fun n0 () Int)
(declare-fun n1 () Int)
(declare-fun n2 () Int)



(assert 
	(and
	(= n0 (+ n1 2))
	(= n1 (+ n2 1))
	(tobool 
 	        (ssep 
                (lls X n0 Y n1)
				(lls Y n1 Z n2)
		)
	)
	)
)


(assert (not
	(and
	(= n0 (+ n2 3))
	(tobool 
 	       (lls X n0 Z n2)
	)
	)
)
)


(check-sat)

;; unsat: entl is true