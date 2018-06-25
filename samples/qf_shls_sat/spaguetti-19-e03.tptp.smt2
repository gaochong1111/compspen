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

			(distinct x11 x13)
			(distinct x11 x15)
			(distinct x3 x6)
			(distinct x7 x9)
			(distinct x9 x18)
			(distinct x9 x19)
			(distinct x12 x18)
			(distinct x12 x19)
			(distinct x2 x16)
			(distinct x2 x13)
			(distinct x8 x18)
			(distinct x8 x10)
			(distinct x1 x6)
			(distinct x1 x7)
			(distinct x1 x10)
			(distinct x1 x16)
			(distinct x1 x17)
			(distinct x5 x16)
			(distinct x5 x13)
		(tobool (ssep 
			(ls x8 x1 )
			(ls x12 x19 )
			(ls x17 x19 )
			(ls x7 x17 )
			(ls x7 x12 )
			(ls x3 x2 )
		)

		)

) )

(check-sat)
;   sat