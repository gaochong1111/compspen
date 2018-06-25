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
(declare-fun E3() Dll_t)


(declare-fun E1_p() Dll_t)
(declare-fun E2_p() Dll_t)
(declare-fun E3_p() Dll_t)




(declare-fun x1() Int)
(declare-fun x2_1() Int)
(declare-fun x2_2() Int)
(declare-fun x3() Int)





;; phi
(assert (and 
		(= x2_1 x2_2)
        (tobool
            (ssep
                (ldll E1 E1_p x1 E2 E2_p x2_1)
                (ldll E2 E2_p x2_2 E3 E3_p x3)
        )))
)

;; psi
(assert
(not (and
     true
     (tobool
        (ldll E1 E1_p x1 E3 E3_p x3)
     )))
)
(check-sat)

;; unsat: entl is true