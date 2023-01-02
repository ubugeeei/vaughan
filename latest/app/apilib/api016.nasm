[BITS 32]

		GLOBAL	alloc_timer

[SECTION .text]

alloc_timer:	; int alloc_timer(void);
		MOV		EDX,16
		INT		0x40
		RET
