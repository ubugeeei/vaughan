[BITS 32]

		GLOBAL	get_key

[SECTION .text]

get_key:		; int get_key(int mode);
		MOV		EDX,15
		MOV		EAX,[ESP+4]	; mode
		INT		0x40
		RET
