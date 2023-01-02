[BITS 32]

		GLOBAL	init_timer

[SECTION .text]

init_timer:		; void init_timer(int timer, int data);
		PUSH	EBX
		MOV		EDX,17
		MOV		EBX,[ESP+ 8]		; timer
		MOV		EAX,[ESP+12]		; data
		INT		0x40
		POP		EBX
		RET
