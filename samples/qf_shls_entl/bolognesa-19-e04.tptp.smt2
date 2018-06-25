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
			(pto x19 (ref next x13 ))
			(ls x1 x18 )
			(pto x11 (ref next x18 ))
			(pto x18 (ref next x16 ))
			(ls x16 x19 )
			(pto x2 (ref next x19 ))
			(ls x4 x12 )
			(pto x6 (ref next x7 ))
			(ls x10 x2 )
			(pto x5 (ref next x8 ))
			(pto x14 (ref next x6 ))
			(pto x3 (ref next x10 ))
			(pto x7 (ref next x18 ))
			(pto x12 (ref next x15 ))
			(pto x9 (ref next x16 ))
			(pto x17 (ref next x5 ))
			(pto x13 (ref next x9 ))
			(pto x15 (ref next x17 ))
			(pto x8 (ref next x7 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x4 x12 )
			(ls x14 x6 )
			(ls x12 x15 )
			(ls x11 x18 )
			(ls x15 x7 )
			(ls x6 x18 )
			(ls x1 x16 )
			(ls x16 x19 )
			(ls x3 x16 )
		)

) ))

(check-sat)
;   sat