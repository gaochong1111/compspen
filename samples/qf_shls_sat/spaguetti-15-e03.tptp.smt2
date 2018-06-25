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

(assert 
		(and 

			(distinct x6 x12)
			(distinct x11 x13)
			(distinct x3 x13)
			(distinct x7 x11)
			(distinct x7 x12)
			(distinct x12 x13)
			(distinct x12 x15)
			(distinct x2 x8)
			(distinct x2 x3)
			(distinct x2 x12)
			(distinct x14 x15)
			(distinct x8 x10)
			(distinct x4 x15)
			(distinct x4 x14)
			(distinct x1 x8)
			(distinct x1 x13)
			(distinct x1 x12)
			(distinct x1 x14)
			(distinct x1 x15)
			(distinct x10 x15)
		(tobool (ssep 
			(ls x5 x1 )
			(ls x10 x12 )
			(ls x4 x1 )
			(ls x14 x2 )
			(ls x12 x2 )
			(ls x12 x3 )
			(ls x2 x12 )
			(ls x9 x13 )
			(ls x9 x7 )
			(ls x7 x6 )
			(ls x11 x12 )
		)

		)

) )

(check-sat)
;   sat