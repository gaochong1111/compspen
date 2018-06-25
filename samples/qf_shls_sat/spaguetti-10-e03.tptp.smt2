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

(assert 
		(and 

			(distinct x1 x3)
			(distinct x4 x6)
			(distinct x4 x7)
			(distinct x3 x8)
			(distinct x7 x10)
			(distinct x7 x9)
			(distinct x2 x8)
			(distinct x2 x7)
			(distinct x2 x9)
			(distinct x5 x7)
		(tobool (ssep 
			(ls x10 x9 )
			(ls x10 x3 )
			(ls x7 x10 )
			(ls x3 x10 )
			(ls x1 x2 )
			(ls x1 x10 )
			(ls x1 x8 )
			(ls x8 x4 )
			(ls x6 x4 )
		)

		)

) )

(check-sat)
;   unsat