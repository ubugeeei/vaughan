[BITS 32]
	GLOBAL	api_putchar, api_putstr0
	GLOBAL	api_end
	GLOBAL	api_openwin

[SECTION .text]

api_putchar:
	MOV		EDX,1
	MOV	 AL,[ESP+4]
	INT	 0x40
	RET

api_putstr0:
	PUSH	EBX
	MOV		EDX,2
	MOV		EBX,[ESP+8] ; s
	INT		0x40
	POP		EBX
	RET

api_end:
	MOV	 EDX,4
	INT		0x40

api_openwin:
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV		EDX,5
	MOV		EBX,[ESP+16] ; buf
	MOV		ESI,[ESP+20] ; xsiz
	MOV		EDI,[ESP+24] ; ysiz
	MOV		EAX,[ESP+28] ; col_inv
	MOV		ECX,[ESP+32] ; title
	INT		0x40
	POP		EBX
	POP		ESI
	POP		EDI
	RET
