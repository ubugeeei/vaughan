[BITS 32]

		GLOBAL	putstr_with_len

[SECTION .text]

putstr_with_len:	; void putstr_with_len(char *s, int l);
		PUSH	EBX
		MOV		EDX,3
		MOV		EBX,[ESP+ 8]	; s
		MOV		ECX,[ESP+12]	; l
		INT		0x40
		POP		EBX
		RET
