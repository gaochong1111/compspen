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
(declare-fun E4() Dll_t)
(declare-fun E5() Dll_t)
(declare-fun E6() Dll_t)

(declare-fun E7() Dll_t)
(declare-fun E8() Dll_t)
(declare-fun E9() Dll_t)


(declare-fun E1_prime() Dll_t)
(declare-fun E2_prime() Dll_t)
(declare-fun E3_prime() Dll_t)
(declare-fun E4_prime() Dll_t)
(declare-fun E5_prime() Dll_t)
(declare-fun E6_prime() Dll_t)

(declare-fun x1() Int)
(declare-fun x2() Int)
(declare-fun x3() Int)
(declare-fun x4() Int)
(declare-fun x5() Int)
(declare-fun x6() Int)
(declare-fun x3_prime() Int)
(declare-fun x4_prime() Int)
(declare-fun x5_prime() Int)



;; phi
(assert (and
    (tobool
        (ssep
            (ldll E1 E1_prime x1 E3 E3_prime x3)
            (ldll E2 E2_prime x2 E4 E4_prime x4)
            (ldll E3 E3_prime x3 E4 E7 x4)
            (ldll E4 E4_prime x4_prime E3 E8 x3_prime)
            (ldll E3 E3_prime x3 E5 E5_prime x5)
            (ldll E5 E5_prime x5_prime E3 E9 x3_prime)
            (ldll E4 E4_prime x4 E6 E6_prime x6)
        ))
        )
)

;; psi
(assert (not (tobool
        (ldll E1 E1_prime x1 E3 E3_prime x3)
)
))

(check-sat)

;; sat : entl is false