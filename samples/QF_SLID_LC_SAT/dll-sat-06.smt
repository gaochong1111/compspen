
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
(declare-fun x1 () Dll_t)
(declare-fun x2 () Dll_t)
(declare-fun x3 () Dll_t)
(declare-fun x4 () Dll_t)
(declare-fun y1 () Dll_t)
(declare-fun y2 () Dll_t)
(declare-fun y3 () Dll_t)
(declare-fun y4 () Dll_t)


(declare-fun d1 () Int)
(declare-fun d2 () Int)
(declare-fun d3 () Int)
(declare-fun d4 () Int)
(declare-fun n1 () Int)
(declare-fun n2 () Int)
(declare-fun n3 () Int)
(declare-fun n4 () Int)




(assert
	(and
		(= n2 (+ n3 1))
		(> n2 (+ n4 1))
		(distinct d1 d2)
		(= x4 y1)
		(tobool
			(ssep
				(lsdll x1 x2 d1 n1 y1 y2 d2 n2)
				(pto y1 (sref (ref next x3) (ref prev y2) (ref data d2)))
				(lsdll x3 x4 d3 n3 y3 y4 d4 n4)
			)
		)
	)
)

(check-sat)

;; unsat
