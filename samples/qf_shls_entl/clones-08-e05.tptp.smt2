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

(assert 
		(and 

			(distinct nil x1)
			(distinct nil x3)
			(distinct nil x5)
			(distinct nil x7)
			(distinct nil x9)
			(distinct nil x11)
			(distinct nil x13)
			(distinct nil x15)
		(tobool (ssep 
			(pto x15 (ref next x16 ))
			(pto x13 (ref next x14 ))
			(pto x11 (ref next x12 ))
			(pto x9 (ref next x10 ))
			(pto x7 (ref next x8 ))
			(pto x5 (ref next x6 ))
			(pto x3 (ref next x4 ))
			(pto x1 (ref next x2 ))
		)

		)

) )

(assert (not 
		(tobool (ssep 
			(pto x15 (ref next x16 ))
			(pto x13 (ref next x14 ))
			(pto x11 (ref next x12 ))
			(pto x9 (ref next x10 ))
			(pto x7 (ref next x8 ))
			(pto x5 (ref next x6 ))
			(pto x3 (ref next x4 ))
			(pto x1 (ref next x2 ))
		)

) ))

(check-sat)
;   unsat