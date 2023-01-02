[BITS 32]

		GLOBAL	puts_with_len

[SECTION .text]

puts_with_len:	; void puts_with_len(char *s, int l);
		PUSH	EBX
		MOV		EDX,3
		MOV		EBX,[ESP+ 8]	; s
		MOV		ECX,[ESP+12]	; l
		INT		0x40
		POP		EBX
		RET
