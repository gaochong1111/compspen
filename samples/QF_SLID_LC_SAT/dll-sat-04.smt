
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
(declare-fun x1 () Dll_t)
(declare-fun x2 () Dll_t)
(declare-fun y1 () Dll_t)
(declare-fun y2 () Dll_t)
(declare-fun z () Dll_t)

(declare-fun d1 () Int)
(declare-fun d2 () Int)




(assert
	(and
		(> d1 (+ d2 3))
		(= x1 y2)
		(tobool
			
			(sdll x1 x2 d1 y1 y2 d2)
			
		)
	)
)

(check-sat)

;; sat
