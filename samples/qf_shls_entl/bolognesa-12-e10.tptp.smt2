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
(declare-fun x9() RefSll_t)
(declare-fun x10() RefSll_t)
(declare-fun x11() RefSll_t)
(declare-fun x12() RefSll_t)
(declare-fun x13() RefSll_t)
(declare-fun x14() RefSll_t)
(declare-fun x15() RefSll_t)
(declare-fun x16() RefSll_t)

(assert 
		(and 

		(tobool (ssep 
			(pto x4 (ref next x3 ))
			(pto x7 (ref next x1 ))
			(ls x9 x2 )
			(ls x3 x11 )
			(ls x12 x8 )
			(pto x6 (ref next x1 ))
			(pto x5 (ref next x10 ))
			(ls x2 x1 )
			(pto x8 (ref next x5 ))
			(pto x10 (ref next x3 ))
			(pto x11 (ref next x12 ))
			(pto x1 (ref next x12 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x4 x3 )
			(ls x6 x1 )
			(ls x11 x12 )
			(ls x9 x2 )
			(ls x2 x1 )
			(ls x7 x1 )
			(ls x1 x11 )
		)

) ))

(check-sat)
;   unsat