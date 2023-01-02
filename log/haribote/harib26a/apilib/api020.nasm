[BITS 32]

		GLOBAL	beep

[SECTION .text]

beep:			; void beep(int tone);
		MOV		EDX,20
		MOV		EAX,[ESP+4]			; tone
		INT		0x40
		RET
