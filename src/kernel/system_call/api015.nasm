[BITS 32]

		GLOBAL	getkey

[SECTION .text]

getkey:		; int getkey(int mode);
		MOV		EDX,15
		MOV		EAX,[ESP+4]	; mode
		INT		0x40
		RET
