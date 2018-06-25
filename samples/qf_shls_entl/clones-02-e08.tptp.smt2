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
			(distinct x1 x3)
			(distinct x2 x3)
			(distinct nil x5)
			(distinct nil x6)
			(distinct nil x7)
			(distinct x5 x7)
			(distinct x6 x7)
		(tobool (ssep 
			(ls x6 x5 )
			(pto x7 (ref next x6 ))
			(pto x5 (ref next x7 ))
			(ls x2 x1 )
			(pto x3 (ref next x2 ))
			(pto x1 (ref next x3 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(pto x8 (ref next x7 ))
			(ls x6 x8 )
			(pto x7 (ref next x6 ))
			(pto x4 (ref next x3 ))
			(ls x2 x4 )
			(pto x3 (ref next x2 ))
		)

) ))

(check-sat)
;   sat