(set-logic QF_SLID_LC)

(declare-sort RefSll_t 0)

(declare-fun next() (Field RefSll_t RefSll_t))

(define-fun ls ((in RefSll_t) (out RefSll_t)) Space (tospace
    (or (and  (= in out) (tobool emp) )
        (exists ((u RefSll_t))
            (and (tobool (ssep (pto in (ref next u)) (ls u out ) )) )
)  ) ) )

;; variables
(declare-fun x0() RefSll_t)
(declare-fun x1() RefSll_t)
(declare-fun x2() RefSll_t)
(declare-fun x3() RefSll_t)
(declare-fun x4() RefSll_t)
(declare-fun x5() RefSll_t)
(declare-fun x6() RefSll_t)
(declare-fun x7() RefSll_t)
(declare-fun x8() RefSll_t)

(assert 
		(and 

			(distinct nil x1)
			(distinct nil x2)
			(distinct nil x3)
			(distinct x1 x4)
			(distinct x1 x2)
			(distinct x2 x4)
			(distinct x3 x4)
			(distinct x2 x3)
		(tobool (ssep 
			(pto x1 (ref next x2 ))
			(ls x3 x1 )
			(ls x4 nil )
			(pto x2 (ref next x4 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x4 nil )
			(pto x2 (ref next x4 ))
			(ls x3 x2 )
		)

) ))

(check-sat)
;   unsat