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

			(distinct x6 x12)
			(distinct x7 x16)
			(distinct x2 x7)
			(distinct x2 x16)
			(distinct x2 x9)
			(distinct x14 x17)
			(distinct x4 x6)
			(distinct x1 x8)
			(distinct x1 x9)
			(distinct x1 x17)
			(distinct x10 x13)
			(distinct x10 x12)
		(tobool (ssep 
			(ls x5 x14 )
			(ls x5 x12 )
			(ls x5 x1 )
			(ls x5 x6 )
			(ls x13 x5 )
			(ls x4 x10 )
			(ls x8 x16 )
			(ls x15 x14 )
			(ls x15 x13 )
			(ls x12 x13 )
			(ls x9 x17 )
			(ls x3 x12 )
			(ls x3 x17 )
			(ls x11 x7 )
			(ls x6 x5 )
		)

		)

) )

(check-sat)
;   sat