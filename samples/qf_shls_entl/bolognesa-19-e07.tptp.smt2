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
			(pto x19 (ref next x14 ))
			(ls x12 x2 )
			(pto x15 (ref next x17 ))
			(ls x16 x11 )
			(ls x10 x1 )
			(ls x5 x10 )
			(pto x4 (ref next x8 ))
			(pto x9 (ref next x19 ))
			(ls x17 x11 )
			(pto x14 (ref next x16 ))
			(pto x7 (ref next x19 ))
			(pto x11 (ref next x8 ))
			(ls x8 x13 )
			(pto x2 (ref next x19 ))
			(ls x1 x18 )
			(pto x13 (ref next x8 ))
			(pto x6 (ref next x18 ))
			(ls x3 x1 )
			(pto x18 (ref next x8 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x4 x8 )
			(ls x15 x17 )
			(ls x7 x19 )
			(ls x6 x18 )
			(ls x3 x1 )
			(ls x13 x8 )
			(ls x12 x19 )
			(ls x5 x10 )
			(ls x17 x11 )
			(ls x10 x8 )
			(ls x9 x13 )
		)

) ))

(check-sat)
;   sat