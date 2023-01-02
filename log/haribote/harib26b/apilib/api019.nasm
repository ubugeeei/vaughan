[BITS 32]

		GLOBAL	clear_timeout

[SECTION .text]

clear_timeout:		; void clear_timeout(int timer);
		PUSH	EBX
		MOV		EDX,19
		MOV		EBX,[ESP+ 8]		; timer
		INT		0x40
		POP		EBX
		RET
