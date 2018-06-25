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

			(distinct x11 x12)
			(distinct x7 x13)
			(distinct x7 x9)
			(distinct x9 x13)
			(distinct x9 x12)
			(distinct x8 x15)
			(distinct x4 x10)
			(distinct x10 x11)
			(distinct x10 x13)
			(distinct x10 x15)
			(distinct x5 x10)
		(tobool (ssep 
			(ls x5 x8 )
			(ls x13 x2 )
			(ls x13 x7 )
			(ls x1 x2 )
			(ls x4 x11 )
			(ls x14 x7 )
			(ls x2 x4 )
			(ls x12 x4 )
			(ls x9 x10 )
			(ls x7 x1 )
			(ls x11 x3 )
		)

		)

) )

(check-sat)
;   sat