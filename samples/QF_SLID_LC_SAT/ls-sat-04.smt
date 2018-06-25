
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
					(< ?dt1 (+ ?dt3 0))
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
(declare-fun y_emp () Sll_t)
(declare-fun w_emp () Sll_t)
(declare-fun z_emp () Sll_t)
(declare-fun d1 () Int)
(declare-fun d2 () Int)


(assert 
	(and
	(distinct d1 d2)
	(tobool 
 	        (ssep 
                (pto y_emp (sref (ref next z_emp) (ref data d1)) )
		        (pto z_emp (sref (ref next y_emp) (ref data d1)) )
			    (sls y_emp d1 w_emp d2)
		)
	)
	)
)

(check-sat)

;; unsat
