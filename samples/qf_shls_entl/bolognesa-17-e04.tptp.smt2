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
			(pto x14 (ref next x7 ))
			(ls x9 x8 )
			(pto x7 (ref next x17 ))
			(ls x8 x11 )
			(pto x5 (ref next x14 ))
			(ls x12 x5 )
			(pto x2 (ref next x16 ))
			(ls x16 x3 )
			(ls x15 x10 )
			(pto x10 (ref next x7 ))
			(pto x6 (ref next x17 ))
			(pto x13 (ref next x3 ))
			(ls x17 x9 )
			(ls x11 x3 )
			(ls x4 x16 )
			(pto x3 (ref next x14 ))
			(pto x1 (ref next x3 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x1 x3 )
			(ls x2 x16 )
			(ls x13 x3 )
			(ls x12 x5 )
			(ls x4 x16 )
			(ls x16 x3 )
			(ls x6 x17 )
			(ls x15 x7 )
			(ls x5 x7 )
			(ls x7 x14 )
		)

) ))

(check-sat)
;   unsat