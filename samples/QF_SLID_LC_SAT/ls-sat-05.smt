
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
(declare-fun y_emp () Sll_t)
(declare-fun w_emp () Sll_t)
(declare-fun z_emp () Sll_t)
(declare-fun u_emp () Sll_t)
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
	(= n3 (+ n4 3))
	(tobool 
 	        (ssep 
                (lsls y_emp d1 n1 u_emp d2 n2)
		        (pto z_emp (sref (ref next y_emp) (ref data d3)) )
			    (lsls y_emp d4 n3 w_emp d5 n4)
		)
	)
	)
)

(check-sat)

;; unsat
