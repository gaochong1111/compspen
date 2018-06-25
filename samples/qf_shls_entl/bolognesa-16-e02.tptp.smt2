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

(assert 
		(and 

		(tobool (ssep 
			(pto x3 (ref next x4 ))
			(pto x8 (ref next x14 ))
			(pto x2 (ref next x11 ))
			(ls x5 x15 )
			(pto x14 (ref next x1 ))
			(pto x12 (ref next x11 ))
			(pto x13 (ref next x1 ))
			(pto x9 (ref next x11 ))
			(pto x16 (ref next x14 ))
			(pto x1 (ref next x2 ))
			(ls x4 x15 )
			(pto x7 (ref next x11 ))
			(pto x10 (ref next x6 ))
			(ls x6 x5 )
			(pto x11 (ref next x1 ))
			(pto x15 (ref next x6 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x13 x1 )
			(ls x12 x11 )
			(ls x8 x14 )
			(ls x16 x14 )
			(ls x3 x15 )
			(ls x2 x11 )
			(ls x9 x11 )
			(ls x14 x1 )
			(ls x15 x6 )
			(ls x10 x15 )
			(ls x7 x2 )
		)

) ))

(check-sat)
;   sat