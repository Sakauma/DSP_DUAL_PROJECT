	.ref    SRIO0Int
	.ref	SRIO1Int
	.ref    _c_int00

VEC_RESET .macro addr
	MVKL  addr,B0
	MVKH  addr,B0
	B     B0
	MVC   PCE1,B0
	MVC   B0,ISTP
	NOP   3
	.align 32
	.endm

VEC_ENTRY .macro addr
	STW   B0,*--B15
	MVKL  addr,B0
	MVKH  addr,B0
	B     B0
	LDW   *B15++,B0
	NOP   4
	.align 32
	.endm

VEC_DUMMY .macro
unused_int?:
	B    unused_int?
	NOP  5
	.align 32
	.endm



	.sect "vecs"
	.align 1024

vectors:
	VEC_RESET _c_int00  ;RESET
	VEC_DUMMY 			;NMI/Exception
	VEC_DUMMY   		;RSVD
	VEC_DUMMY   		;RSVD
	VEC_DUMMY   		;interrupt 4
	VEC_ENTRY SRIO0Int  ;interrupt 5
	VEC_ENTRY SRIO1Int	;interrupt 6
	VEC_DUMMY   		;interrupt 7
	VEC_DUMMY			;interrupt 8
	VEC_DUMMY   		;interrupt 9
	VEC_DUMMY   		;interrupt 10
	VEC_DUMMY   		;interrupt 11
	VEC_DUMMY   		;interrupt 12
	VEC_DUMMY   		;interrupt 13
	VEC_DUMMY			;interrupt 14
	VEC_DUMMY   		;interrupt 15
	.end
