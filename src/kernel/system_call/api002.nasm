[BITS 32]

		GLOBAL	puts

[SECTION .text]

puts:	; void puts(char *s);
		PUSH	EBX
		MOV		EDX,2
		MOV		EBX,[ESP+8]		; s
		INT		0x40
		POP		EBX
		RET
