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
(declare-fun x24() RefSll_t)

(assert 
		(and 

		(tobool (ssep 
			(pto x16 (ref next x2 ))
			(pto x2 (ref next x16 ))
			(ls x7 x18 )
			(ls x19 x13 )
			(pto x18 (ref next x14 ))
			(pto x17 (ref next x10 ))
			(ls x8 x1 )
			(pto x4 (ref next x16 ))
			(pto x1 (ref next x20 ))
			(pto x20 (ref next x17 ))
			(ls x13 x4 )
			(pto x14 (ref next x20 ))
			(pto x11 (ref next x18 ))
			(pto x15 (ref next x6 ))
			(pto x9 (ref next x2 ))
			(pto x10 (ref next x6 ))
			(pto x3 (ref next x2 ))
			(pto x12 (ref next x20 ))
			(pto x5 (ref next x3 ))
			(pto x6 (ref next x4 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x7 x18 )
			(ls x15 x6 )
			(ls x11 x18 )
			(ls x9 x2 )
			(ls x8 x1 )
			(ls x1 x20 )
			(ls x12 x20 )
			(ls x19 x13 )
			(ls x18 x4 )
			(ls x13 x4 )
			(ls x5 x16 )
			(ls x4 x2 )
		)

) ))

(check-sat)
;   unsat