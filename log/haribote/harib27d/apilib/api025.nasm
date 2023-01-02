[BITS 32]

	GLOBAL	fread

[SECTION .text]

fread: ; int fread(char *buf, int maxsize, int fhandle)
	PUSH	EBX
	MOV		EDX,25
	MOV		EAX,[ESP+16] ; fhandle
	MOV		ECX,[ESP+12] ; maxsize
	MOV		EBX,[ESP+8] ; buf
	INT		0x40
	POP		EBX
	RET
