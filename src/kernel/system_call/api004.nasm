[BITS 32]

		GLOBAL	exit

[SECTION .text]

exit:	; void exit(void);
		MOV		EDX,4
		INT		0x40
