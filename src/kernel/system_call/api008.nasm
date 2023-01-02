[BITS 32]

		GLOBAL	init_malloc

[SECTION .text]

init_malloc:	; void init_malloc(void);
		PUSH	EBX
		MOV		EDX,8
		MOV		EBX,[CS:0x0020]
		MOV		EAX,EBX
		ADD		EAX,32*1024			; 32KB
		MOV		ECX,[CS:0x0000]
		SUB		ECX,EAX
		INT		0x40
		POP		EBX
		RET
