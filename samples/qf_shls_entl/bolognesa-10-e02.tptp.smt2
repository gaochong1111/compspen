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

		(tobool (ssep 
			(pto x5 (ref next x10 ))
			(ls x9 x7 )
			(pto x2 (ref next x7 ))
			(pto x4 (ref next x7 ))
			(pto x6 (ref next x4 ))
			(pto x7 (ref next x3 ))
			(pto x1 (ref next x2 ))
			(pto x10 (ref next x1 ))
			(pto x8 (ref next x4 ))
			(pto x3 (ref next x5 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(ls x3 x5 )
			(ls x8 x4 )
			(ls x9 x7 )
			(ls x6 x7 )
			(ls x5 x3 )
		)

) ))

(check-sat)
;   unsat