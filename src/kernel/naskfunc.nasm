[BITS 32]

GLOBAL	asm_io_hlt, asm_io_cli, asm_io_sti, asm_io_stihlt
GLOBAL	asm_io_in8,  io_in16,  io_in32
GLOBAL	asm_io_out8, io_out16, io_out32
GLOBAL	asm_io_load_eflags, asm_io_store_eflags
GLOBAL	asm_load_gdtr, asm_load_idtr
GLOBAL	asm_load_cr0, asm_store_cr0
GLOBAL  asm_load_tr
GLOBAL	asm_inthandler20, asm_inthandler21, asm_inthandler27, asm_inthandler2c, asm_inthandler0c, asm_inthandler0d
GLOBAL	asm_memtest_sub
GLOBAL  asm_far_jmp, asm_far_call
GLOBAL	asm_os_api
GLOBAL	asm_start_app, asm_end_app

EXTERN	inthandler0c, inthandler0d, inthandler20, inthandler21, inthandler27, inthandler2c
EXTERN	hrb_api

[SECTION .text]

asm_io_hlt:
	HLT
	RET

asm_io_cli:
	CLI
	RET

asm_io_sti:
	STI
	RET

asm_io_stihlt:
	STI
	HLT
	RET

asm_io_in8:
	MOV		EDX,[ESP+4]
	MOV		EAX,0
	IN		AL,DX
	RET

io_in16:
	MOV		EDX,[ESP+4]
	MOV		EAX,0
	IN		AX,DX
	RET

io_in32:
	MOV		EDX,[ESP+4]
	IN		EAX,DX
	RET

asm_io_out8:
	MOV		EDX,[ESP+4]
	MOV		AL,[ESP+8]
	OUT		DX,AL
	RET

io_out16:
	MOV		EDX,[ESP+4]
	MOV		EAX,[ESP+8]
	OUT		DX,AX
	RET

io_out32:
	MOV		EDX,[ESP+4]
	MOV		EAX,[ESP+8]
	OUT		DX,EAX
	RET

asm_io_load_eflags:
	PUSHFD
	POP		EAX
		RET

asm_io_store_eflags:
	MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD
		RET

asm_load_gdtr:
	MOV	AX,[ESP+4]
	MOV	[ESP+6],AX
	LGDT	[ESP+6]
	RET

asm_load_idtr:
	MOV	AX,[ESP+4]
	MOV	[ESP+6],AX
	LIDT	[ESP+6]
	RET

asm_load_cr0:		; int asm_load_cr0(void);
	MOV		EAX,CR0
	RET

asm_store_cr0:		; void asm_store_cr0(int cr0);
	MOV		EAX,[ESP+4]
	MOV		CR0,EAX
	RET

asm_load_tr:		; void asm_load_tr(int tr);
	LTR		[ESP+4]
	RET

asm_inthandler20:
  PUSH	ES
  PUSH	DS
  PUSHAD
  MOV		EAX,ESP
  PUSH	EAX
  MOV		AX,SS
  MOV		DS,AX
  MOV		ES,AX
  CALL	inthandler20
  POP		EAX
  POPAD
  POP		DS
  POP		ES
  IRETD

asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler0c:
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler0c
		CMP		EAX,0
		JNE		asm_end_app
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		ADD		ESP,4
		IRETD

asm_inthandler0d:
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler0d
		CMP		EAX,0
		JNE		asm_end_app
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		ADD		ESP,4
		IRETD

asm_far_jmp: ; void asm_far_jmp(int eip, int cs);
		JMP FAR [ESP+4]
		RET
asm_far_call:
		CALL	FAR [ESP+4]
		RET

asm_memtest_sub:	; unsigned int asm_memtest_sub(unsigned int start, unsigned int end)
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV	ESI,0xaa55aa55			; pat0 = 0xaa55aa55;
	MOV	EDI,0x55aa55aa			; pat1 = 0x55aa55aa;
	MOV	EAX,[ESP+12+4]			; i = start;
mts_loop:
	MOV	EBX,EAX
	ADD	EBX,0xffc			; p = i + 0xffc;
	MOV	EDX,[EBX]			; old = *p;
	MOV	[EBX],ESI			; *p = pat0;
	XOR	DWORD [EBX],0xffffffff		; *p ^= 0xffffffff;
	CMP	EDI,[EBX]			; if (*p != pat1) goto fin;
	JNE	mts_fin
	XOR	DWORD [EBX],0xffffffff		; *p ^= 0xffffffff;
	CMP	ESI,[EBX]			; if (*p != pat0) goto fin;
	JNE	mts_fin
	MOV	[EBX],EDX			; *p = old;
	ADD	EAX,0x1000			; i += 0x1000;
	CMP	EAX,[ESP+12+8]			; if (i <= end) goto mts_loop;
	JBE	mts_loop
	POP	EBX
	POP	ESI
	POP	EDI
	RET
mts_fin:
	MOV	[EBX],EDX			; *p = old;
	POP	EBX
	POP	ESI
	POP	EDI
	RET

asm_os_api:
		STI
		PUSH	DS
		PUSH	ES
		PUSHAD
		PUSHAD
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	hrb_api
		CMP		EAX,0
		JNE		asm_end_app
		ADD		ESP,32
		POPAD
		POP		ES
		POP		DS
		IRETD

asm_end_app:
		MOV		ESP,[EAX]
		MOV		DWORD [EAX+4],0
		POPAD
		RET

asm_start_app:
		PUSHAD
		MOV		EAX,[ESP+36]
		MOV		ECX,[ESP+40]
		MOV		EDX,[ESP+44]
		MOV		EBX,[ESP+48]
		MOV		EBP,[ESP+52]
		MOV		[EBP],ESP
		MOV		[EBP+4],SS
		MOV		ES,BX
		MOV		DS,BX
		MOV		FS,BX
		MOV		GS,BX

		OR		ECX,3
		OR		EBX,3
		PUSH	EBX
		PUSH	EDX
		PUSH	ECX
		PUSH	EAX
		RETF
