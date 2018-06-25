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
(declare-fun x18() RefSll_t)
(declare-fun x19() RefSll_t)
(declare-fun x20() RefSll_t)
(declare-fun x21() RefSll_t)
(declare-fun x22() RefSll_t)
(declare-fun x23() RefSll_t)

(assert 
		(and 

		(tobool (ssep 
			(pto x6 (ref next x18 ))
			(pto x11 (ref next x5 ))
			(pto x17 (ref next x18 ))
			(ls x2 x10 )
			(ls x14 x10 )
			(pto x19 (ref next x17 ))
			(pto x1 (ref next x13 ))
			(pto x10 (ref next x16 ))
			(pto x7 (ref next x8 ))
			(pto x16 (ref next x1 ))
			(pto x13 (ref next x14 ))
			(pto x12 (ref next x6 ))
			(pto x4 (ref next x9 ))
			(pto x15 (ref next x10 ))
			(pto x8 (ref next x9 ))
			(pto x9 (ref next x17 ))
			(pto x5 (ref next x13 ))
			(pto x3 (ref next x9 ))
			(pto x18 (ref next x17 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x12 x6 )
			(ls x2 x10 )
			(ls x11 x5 )
			(ls x1 x13 )
			(ls x15 x10 )
			(ls x4 x9 )
			(ls x19 x17 )
			(ls x7 x8 )
			(ls x8 x9 )
			(ls x6 x18 )
			(ls x3 x18 )
			(ls x18 x17 )
			(ls x5 x1 )
		)

) ))

(check-sat)
;   unsat