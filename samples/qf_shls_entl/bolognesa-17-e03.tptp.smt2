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

(assert 
		(and 

		(tobool (ssep 
			(pto x14 (ref next x12 ))
			(ls x2 x15 )
			(ls x17 x13 )
			(pto x11 (ref next x4 ))
			(pto x5 (ref next x17 ))
			(pto x3 (ref next x11 ))
			(pto x8 (ref next x6 ))
			(ls x6 x5 )
			(ls x10 x4 )
			(ls x16 x2 )
			(ls x7 x14 )
			(pto x1 (ref next x17 ))
			(pto x12 (ref next x15 ))
			(pto x4 (ref next x14 ))
			(pto x13 (ref next x8 ))
			(pto x9 (ref next x5 ))
			(pto x15 (ref next x13 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x1 x17 )
			(ls x9 x5 )
			(ls x7 x14 )
			(ls x17 x13 )
			(ls x16 x15 )
			(ls x3 x4 )
			(ls x10 x17 )
		)

) ))

(check-sat)
;   sat