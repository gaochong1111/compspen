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

			(distinct x6 x15)
			(distinct x11 x13)
			(distinct x7 x8)
			(distinct x7 x13)
			(distinct x2 x5)
			(distinct x8 x11)
			(distinct x8 x12)
			(distinct x8 x15)
			(distinct x1 x6)
			(distinct x1 x13)
			(distinct x1 x9)
			(distinct x1 x2)
			(distinct x1 x5)
			(distinct x4 x6)
			(distinct x4 x9)
			(distinct x4 x13)
			(distinct x13 x14)
		(tobool (ssep 
			(ls x1 x11 )
			(ls x8 x2 )
			(ls x8 x10 )
			(ls x8 x11 )
			(ls x2 x13 )
			(ls x12 x10 )
			(ls x3 x9 )
			(ls x3 x10 )
			(ls x3 x1 )
			(ls x11 x2 )
			(ls x6 x15 )
			(ls x6 x13 )
		)

		)

) )

(check-sat)
;   unsat