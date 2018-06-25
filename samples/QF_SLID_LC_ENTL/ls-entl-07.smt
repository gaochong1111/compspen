
(set-logic QF_SLID_LC)

;; declare sorts
(declare-sort Sll_t 0)


;; declare fields
(declare-fun next () (Field Sll_t Sll_t))
(declare-fun data () (Field Sll_t Int))

;; declare predicates

(define-fun lsls 
	((?in Sll_t) (?dt1 Int) (?len1 Int) (?out Sll_t) (?dt2 Int) (?len2 Int)) 
	Space 
	(tospace 
		(or 
			(and (= ?in ?out) (= ?dt1 ?dt2) (= ?len1 ?len2) 
			(tobool emp)
			)

 
			(exists 
				((?u Sll_t) (?dt3 Int) (?len3 Int)) 
				(and  	
					(< ?dt1 (+ ?dt3 0))
					(= ?len1 (+ ?len3 1))
					(tobool (ssep 
						(pto ?in (sref (ref next ?u) (ref data ?dt1)) ) 
						(lsls ?u ?dt3 ?len3 ?out ?dt2 ?len2)
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
(declare-fun U () Sll_t)
(declare-fun d1 () Int)
(declare-fun d2 () Int)
(declare-fun d3 () Int)
(declare-fun d4 () Int)
(declare-fun d5 () Int)
(declare-fun n1 () Int)
(declare-fun n2 () Int)
(declare-fun n3 () Int)
(declare-fun n4 () Int)


(assert 
	(and
	(distinct d1 d2)
	(= d2 d3)
	(= n3 (+ n4 2))
	(= n2 (+ n3 1))
	(< d3 d4)
	(tobool 
 	        (ssep 
                (lsls X d1 n1 Y d2 n2)
		        (pto Y (sref (ref next Z) (ref data d3)) )
			    (lsls Z d4 n3 U d5 n4)
		)
	)
	)
)

(assert (not 
	(and
	(< d1 d5)
	(> n1 (+ n4 3))
	(tobool 
 	       (lsls X d1 n1 U d5 n4)
	)
	)
)
)


(check-sat)

;; unsat： entl is true
