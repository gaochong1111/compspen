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

(assert 
		(and 

			(distinct nil x1)
			(distinct nil x2)
			(distinct nil x3)
			(distinct nil x4)
			(distinct nil x5)
			(distinct nil x6)
			(distinct nil x7)
			(distinct x2 x7)
			(distinct x3 x7)
			(distinct x4 x5)
			(distinct x4 x6)
		(tobool (ssep 
			(ls x1 x7 )
			(pto x7 (ref next x1 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x8 x7 )
			(pto x7 (ref next x8 ))
		)

) ))

(check-sat)
;   sat