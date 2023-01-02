[BITS 32]

		GLOBAL	close_window

[SECTION .text]

close_window:		; void close_window(int win);
		PUSH	EBX
		MOV		EDX,14
		MOV		EBX,[ESP+8]	; win
		INT		0x40
		POP		EBX
		RET
