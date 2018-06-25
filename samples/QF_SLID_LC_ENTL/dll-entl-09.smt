(set-logic QF_SLID_LC)

;; declare sorts
(declare-sort Dll_t 0)


;; declare fields
(declare-fun next () (Field Dll_t Dll_t))
(declare-fun prev () (Field Dll_t Dll_t))


;; declare predicates

(define-fun ldll ((?E Dll_t) (?P Dll_t) (?len1 Int) (?F Dll_t) (?L Dll_t) (?len2 Int)) Space 
(tospace 
	(or 
	(and (= ?E ?F) (= ?P ?L) (= ?len1 ?len2)
		(tobool emp
		)
	)
 
	(exists ((?u Dll_t) (?len3 Int)) 
		(and 
			(= ?len1 (+ ?len3 1)) 
			(tobool (ssep 
				(pto ?E (sref (ref next ?u) (ref prev ?P) ) ) 
				(ldll ?u ?E ?len3 ?F ?L ?len2)
			))
		)
	)

	)
))

;; declare variables
(declare-fun E1() Dll_t)
(declare-fun E2() Dll_t)


(declare-fun E1_p() Dll_t)
(declare-fun E2_p() Dll_t)



(declare-fun x1() Int)
(declare-fun x2() Int)





;; phi
(assert (and
        (= x1 (+ x2 1))
        (= E1 E2_p)
        (tobool
            (pto E1 (sref (ref next E2) (ref prev E1_p)))
        ))
)

;; psi
(assert
(not (and 
	 (= x1 (+ x2 1))
     (tobool
        (ldll E1 E1_p x1 E2 E2_p x2)
)))

)

(check-sat)

;; unsat : entl is true