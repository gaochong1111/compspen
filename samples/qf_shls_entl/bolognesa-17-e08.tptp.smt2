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
			(pto x17 (ref next x11 ))
			(pto x9 (ref next x8 ))
			(pto x16 (ref next x11 ))
			(pto x13 (ref next x14 ))
			(ls x14 x3 )
			(pto x6 (ref next x10 ))
			(ls x1 x16 )
			(pto x4 (ref next x1 ))
			(ls x8 x4 )
			(pto x2 (ref next x9 ))
			(pto x10 (ref next x5 ))
			(pto x5 (ref next x11 ))
			(pto x3 (ref next x14 ))
			(pto x11 (ref next x17 ))
			(pto x12 (ref next x4 ))
			(pto x15 (ref next x3 ))
			(pto x7 (ref next x11 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x12 x4 )
			(ls x15 x3 )
			(ls x6 x11 )
			(ls x7 x11 )
			(ls x13 x14 )
			(ls x14 x3 )
			(ls x2 x1 )
			(ls x1 x16 )
			(ls x17 x11 )
			(ls x16 x17 )
			(ls x3 x14 )
		)

) ))

(check-sat)
;   sat