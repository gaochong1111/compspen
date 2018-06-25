
(set-logic QF_SLID_LC)

;; declare sorts
(declare-sort Dll_t 0)


;; declare fields
(declare-fun next () (Field Dll_t Dll_t))
(declare-fun prev () (Field Dll_t Dll_t))
(declare-fun data () (Field Dll_t Int))


;; declare predicates

(define-fun lsdll ((?E Dll_t) (?P Dll_t) (?dt1 Int) (?len1 Int) (?F Dll_t) (?L Dll_t) (?dt2 Int) (?len2 Int)) Space 
(tospace 
	(or 
	(and (= ?E ?F) (= ?P ?L) (= ?dt1 ?dt2) (= ?len1 ?len2)
		(tobool emp
		)
	)

	(exists ((?u Dll_t) (?dt3 Int) (?len3 Int)) 
		(and 
			(> ?dt1 (+ ?dt3 1)) 
			(= ?len1 (+ ?len3 1))
			(tobool (ssep 
				(pto ?E (sref (ref next ?u) (ref prev ?P) (ref data ?dt1) ) ) 
				(lsdll ?u ?E ?dt3 ?len3 ?F ?L ?dt2 ?len2)
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

(declare-fun u1() Int)
(declare-fun u2() Int)
(declare-fun u3() Int)
(declare-fun u4() Int)
(declare-fun u5() Int)
(declare-fun u6() Int)


(declare-fun y3() Int)
(declare-fun y4() Int)
(declare-fun y5() Int)

(assert
	(and
		(distinct u4 u3)
		(tobool
		(ssep  (lsdll E1 F1 x1 u1 E3 F3 x3 u3)
		        (lsdll E2 F2 x2 u2 E4 F4 x4 u4)
		        (lsdll E3 F3 x3 u3 E4 F4 x4 u4)
		        (lsdll E4 F4 y4 u4 E3 F3 y3 u3)
		        (lsdll E3 F3 x3 u3 E5 F5 x5 u5)
		        (lsdll E5 F5 y5 u5 E3 F3 y3 u3)
		        (lsdll E4 F4 x5 u4 E6 F6 x6 u6)
		) 
		)
	)
)


(assert
	(not
		(and
			(tobool
				(ssep  (lsdll E1 F1 x1 u1 E3 F3 x3 u3)
				       (lsdll E2 F2 x2 u2 E6 F6 x6 u6)
				)
			)
		)
	)
)

(check-sat)
;; unsat 
