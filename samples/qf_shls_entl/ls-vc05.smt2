(set-logic QF_SLID_LC)

(declare-sort RefSll_t 0)

(declare-fun next() (Field RefSll_t RefSll_t))

(define-fun ls ((in RefSll_t) (out RefSll_t)) Space (tospace
    (or (and  (= in out) (tobool emp) )
        (exists ((u RefSll_t))
            (and (tobool (ssep (pto in (ref next u)) (ls u out ) )) )
)  ) ) )

;; variables
(declare-fun y_emp() RefSll_t)
(declare-fun w_emp() RefSll_t)

(assert 
		(tobool
			(ls y_emp w_emp )
		)
)

(assert (not 
		(tobool
			(ls y_emp w_emp )
		)
			
))

(check-sat)
;   unsat