[BITS 32]

		GLOBAL	set_timeout

[SECTION .text]

set_timeout:		; void set_timeout(int timer, int time);
		PUSH	EBX
		MOV		EDX,18
		MOV		EBX,[ESP+ 8]		; timer
		MOV		EAX,[ESP+12]		; time
		INT		0x40
		POP		EBX
		RET
