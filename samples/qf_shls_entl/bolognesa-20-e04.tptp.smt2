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
			(pto x15 (ref next x6 ))
			(ls x5 x4 )
			(pto x16 (ref next x9 ))
			(pto x2 (ref next x20 ))
			(pto x11 (ref next x3 ))
			(pto x12 (ref next x19 ))
			(pto x17 (ref next x14 ))
			(pto x14 (ref next x15 ))
			(pto x19 (ref next x10 ))
			(pto x13 (ref next x1 ))
			(ls x8 x13 )
			(pto x10 (ref next x9 ))
			(pto x1 (ref next x3 ))
			(ls x9 x15 )
			(pto x6 (ref next x5 ))
			(ls x7 x3 )
			(pto x18 (ref next x12 ))
			(ls x20 x7 )
			(pto x4 (ref next x1 ))
			(ls x3 x16 )
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x8 x13 )
			(ls x18 x19 )
			(ls x2 x20 )
			(ls x11 x3 )
			(ls x20 x7 )
			(ls x19 x9 )
			(ls x7 x3 )
			(ls x13 x1 )
			(ls x9 x15 )
			(ls x17 x9 )
		)

) ))

(check-sat)
;   sat