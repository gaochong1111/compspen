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

			(distinct x6 x8)
			(distinct x6 x12)
			(distinct x11 x18)
			(distinct x3 x8)
			(distinct x7 x17)
			(distinct x17 x18)
			(distinct x2 x5)
			(distinct x14 x19)
			(distinct x8 x16)
			(distinct x4 x18)
			(distinct x4 x17)
			(distinct x18 x19)
			(distinct x5 x11)
		(tobool (ssep 
			(ls x19 x5 )
			(ls x19 x4 )
			(ls x18 x4 )
			(ls x1 x13 )
			(ls x8 x2 )
			(ls x8 x10 )
			(ls x2 x15 )
			(ls x2 x17 )
			(ls x2 x10 )
			(ls x17 x9 )
			(ls x17 x1 )
			(ls x9 x18 )
			(ls x7 x19 )
			(ls x3 x13 )
			(ls x11 x16 )
		)

		)

) )

(check-sat)
;   sat