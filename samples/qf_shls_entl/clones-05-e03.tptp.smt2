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

			(distinct nil x1)
			(distinct nil x2)
			(distinct x1 x2)
			(distinct nil x3)
			(distinct nil x4)
			(distinct x3 x4)
			(distinct nil x5)
			(distinct nil x6)
			(distinct x5 x6)
			(distinct nil x7)
			(distinct nil x8)
			(distinct x7 x8)
			(distinct nil x9)
			(distinct nil x10)
			(distinct x9 x10)
			(tobool emp)
		)

)

(assert (not 
			(tobool emp)
))

(check-sat)
;   unsat