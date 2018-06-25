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

			(distinct x6 x11)
			(distinct x6 x14)
			(distinct x3 x8)
			(distinct x3 x15)
			(distinct x7 x8)
			(distinct x14 x16)
			(distinct x8 x11)
			(distinct x8 x15)
			(distinct x1 x14)
			(distinct x13 x15)
			(distinct x10 x11)
			(distinct x10 x13)
			(distinct x5 x16)
			(distinct x5 x9)
		(tobool (ssep 
			(ls x13 x5 )
			(ls x13 x2 )
			(ls x10 x3 )
			(ls x1 x10 )
			(ls x4 x12 )
			(ls x2 x15 )
			(ls x2 x17 )
			(ls x2 x16 )
			(ls x12 x16 )
			(ls x9 x14 )
			(ls x7 x1 )
			(ls x3 x9 )
			(ls x6 x2 )
		)

		)

) )

(check-sat)
;   sat