[BITS 32]

	GLOBAL	fsize

[SECTION .text]

fsize: ; int fsize(int fhandle, int mode)
	MOV		EDX,24
	MOV		EAX,[ESP+4] ; fhandle
	MOV		ECX,[ESP+8] ; mode
	INT		0x40
	RET
