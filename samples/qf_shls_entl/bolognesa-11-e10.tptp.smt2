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

		(tobool (ssep 
			(ls x1 x3 )
			(pto x8 (ref next x11 ))
			(pto x5 (ref next x8 ))
			(pto x11 (ref next x10 ))
			(ls x3 x1 )
			(ls x10 x9 )
			(pto x4 (ref next x6 ))
			(pto x6 (ref next x4 ))
			(pto x9 (ref next x10 ))
			(pto x7 (ref next x1 ))
			(ls x2 x6 )
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x2 x4 )
			(ls x9 x10 )
			(ls x7 x3 )
			(ls x4 x6 )
			(ls x3 x1 )
			(ls x5 x11 )
			(ls x11 x9 )
		)

) ))

(check-sat)
;   sat