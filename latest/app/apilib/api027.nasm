[BITS 32]

	GLOBAL get_lang_mode

[SECTION .text]

get_lang_mode: ; int get_lang_mode(void)
	MOV		EDX,27
	INT		0x40
	RET
