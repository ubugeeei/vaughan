[BITS 32]

		GLOBAL	putstr

[SECTION .text]

putstr:	; void putstr(char *s);
		PUSH	EBX
		MOV		EDX,2
		MOV		EBX,[ESP+8]		; s
		INT		0x40
		POP		EBX
		RET
