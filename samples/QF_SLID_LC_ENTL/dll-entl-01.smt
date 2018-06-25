
(set-logic QF_SLID_LC)

;; declare sorts
(declare-sort Dll_t 0)


;; declare fields
(declare-fun next () (Field Dll_t Dll_t))
(declare-fun prev () (Field Dll_t Dll_t))
(declare-fun data () (Field Dll_t Int))


;; declare predicates

(define-fun sdll ((?E Dll_t) (?P Dll_t) (?dt1 Int) (?F Dll_t) (?L Dll_t) (?dt2 Int)) Space 
(tospace 
	(or 
	(and (= ?E ?F) (= ?P ?L) (= ?dt1 ?dt2)
		(tobool emp
		)
	)

	(exists ((?u Dll_t) (?dt3 Int)) 
		(and 
			(> ?dt1 (+ ?dt3 1)) 
			(tobool (ssep 
				(pto ?E (sref (ref next ?u) (ref prev ?P) (ref data ?dt1) ) ) 
				(sdll ?u ?E ?dt3 ?F ?L ?dt2)
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

(declare-fun F1() Dll_t)
(declare-fun F2() Dll_t)
(declare-fun F3() Dll_t)
(declare-fun F4() Dll_t)
(declare-fun F5() Dll_t)
(declare-fun F6() Dll_t)

(declare-fun x1() Int)
(declare-fun x2() Int)
(declare-fun x3() Int)
(declare-fun x4() Int)
(declare-fun x5() Int)
(declare-fun x6() Int)



(declare-fun y3() Int)
(declare-fun y4() Int)
(declare-fun y5() Int)


(assert
	(and
        (= x4 x5)
        (= x4 y4)
        (= x3 y3)
        (= x5 y5)
		(tobool
		(ssep   (sdll E1 F1 x1 E3 F3 x3)
		     (sdll E2 F2 x2 E4 F4 x4)
			 (sdll E3 F3 x3 E4 F4 x4)
			 (sdll E4 F4 y4 E3 F3 y3)
			 (sdll E3 F3 x3 E5 F5 x5)
			 (sdll E5 F5 y5 E3 F3 y3)
			 (sdll E4 F4 x5 E6 F6 x6)
		) 
		)
	)
)


(assert
	(not
		(and
			(tobool
				(ssep  (sdll E1 F1 x1 E3 F3 x3)
					   (sdll E2 F2 x2 E6 F6 x6)
				)
			)
		)
	)
)


(check-sat)
; unsat: entl is true