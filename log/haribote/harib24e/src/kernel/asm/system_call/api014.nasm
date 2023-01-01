[BITS 32]

		GLOBAL	close

[SECTION .text]

close:		; void close(int win);
		PUSH	EBX
		MOV		EDX,14
		MOV		EBX,[ESP+8]	; win
		INT		0x40
		POP		EBX
		RET
