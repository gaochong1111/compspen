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
(declare-fun x17() RefSll_t)

(assert 
		(and 

		(tobool (ssep 
			(pto x4 (ref next x8 ))
			(pto x7 (ref next x5 ))
			(pto x1 (ref next x10 ))
			(pto x3 (ref next x7 ))
			(ls x5 x12 )
			(ls x10 x13 )
			(ls x13 x9 )
			(pto x8 (ref next x11 ))
			(ls x12 x4 )
			(pto x6 (ref next x12 ))
			(pto x2 (ref next x4 ))
			(ls x9 x11 )
			(pto x11 (ref next x6 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x1 x13 )
			(ls x2 x4 )
			(ls x13 x11 )
			(ls x11 x6 )
			(ls x3 x12 )
			(ls x6 x11 )
		)

) ))

(check-sat)
;   unsat