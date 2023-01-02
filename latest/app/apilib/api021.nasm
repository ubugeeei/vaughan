[BITS 32]

	GLOBAL	fopen

[SECTION .text]

fopen: ; int fopen(char *fname)
	PUSH	EBX
	MOV		EDX,21
	MOV		EBX,[ESP+8] ; fname
	INT		0x40
	POP		EBX
	RET
