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

(assert 
		(and 

			(distinct x8 x9)
			(distinct x8 x10)
			(distinct x6 x11)
			(distinct x1 x11)
			(distinct x1 x5)
			(distinct x4 x7)
			(distinct x4 x5)
			(distinct x7 x9)
			(distinct x2 x10)
		(tobool (ssep 
			(ls x10 x1 )
			(ls x10 x6 )
			(ls x1 x5 )
			(ls x4 x2 )
			(ls x8 x6 )
			(ls x9 x7 )
			(ls x9 x4 )
			(ls x9 x6 )
			(ls x3 x7 )
			(ls x11 x2 )
			(ls x11 x7 )
		)

		)

) )

(check-sat)
;   unsat