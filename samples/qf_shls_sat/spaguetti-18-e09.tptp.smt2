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

(assert 
		(and 

			(distinct x11 x12)
			(distinct x11 x14)
			(distinct x7 x8)
			(distinct x7 x18)
			(distinct x2 x6)
			(distinct x2 x8)
			(distinct x2 x15)
			(distinct x1 x8)
			(distinct x1 x3)
			(distinct x1 x9)
			(distinct x1 x16)
			(distinct x13 x14)
			(distinct x16 x17)
			(distinct x6 x8)
			(distinct x6 x10)
			(distinct x3 x12)
			(distinct x9 x16)
			(distinct x9 x13)
			(distinct x9 x17)
			(distinct x12 x17)
			(distinct x15 x17)
			(distinct x14 x15)
			(distinct x8 x14)
			(distinct x4 x7)
			(distinct x4 x16)
			(distinct x10 x11)
			(distinct x5 x18)
			(distinct x5 x10)
		(tobool (ssep 
			(ls x13 x7 )
			(ls x13 x4 )
			(ls x13 x6 )
			(ls x10 x8 )
			(ls x18 x14 )
			(ls x18 x6 )
			(ls x8 x5 )
			(ls x17 x5 )
			(ls x17 x16 )
			(ls x2 x3 )
			(ls x9 x17 )
			(ls x7 x10 )
			(ls x3 x13 )
			(ls x3 x10 )
			(ls x11 x15 )
		)

		)

) )

(check-sat)
;   unsat