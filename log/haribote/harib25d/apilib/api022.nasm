[BITS 32]

	GLOBAL	fclose

[SECTION .text]

fclose: ; void fclose(int fhandle)
	MOV		EDX,22
	MOV		EAX,[ESP+4] ; fhandle
	INT		0x40
	RET
