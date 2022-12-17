[BITS 32]
	GLOBAL	open, close, putstr_window
	GLOBAL  box_fill_window, draw_point_window, refresh_window, draw_line_window
	GLOBAL	putchar, putstr
	GLOBAL	get_key
	GLOBAL	init_malloc, malloc, free
	GLOBAL  alloc_timer, init_timer, set_timeout, api_freetimer
	GLOBAL  beep
	GLOBAL	api_end

[SECTION .text]

open:
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

close:
	PUSH	EBX
	MOV		EDX,14
	MOV		EBX,[ESP+8] ; win
	INT		0x40
	POP		EBX
	RET

putchar:
	MOV		EDX,1
	MOV	 AL,[ESP+4]
	INT	 0x40
	RET

putstr:
	PUSH	EBX
	MOV		EDX,2
	MOV		EBX,[ESP+8] ; s
	INT		0x40
	POP		EBX
	RET

putstr_window:
	PUSH	EDI
	PUSH	ESI
	PUSH	EBP
	PUSH	EBX
	MOV		EDX,6
	MOV		EBX,[ESP+20] ; win
	MOV		ESI,[ESP+24] ; x
	MOV		EDI,[ESP+28] ; y
	MOV		EAX,[ESP+32] ; col
	MOV		ECX,[ESP+36] ; len
	MOV		EBP,[ESP+40] ; str
	INT		0x40
	POP		EBX
	POP		EBP
	POP		ESI
	POP		EDI
	RET

box_fill_window:
	PUSH	EDI
	PUSH	ESI
	PUSH	EBP
	PUSH	EBX
	MOV		EDX,7
	MOV		EBX,[ESP+20] ; win
	MOV		EAX,[ESP+24] ; x0
	MOV		ECX,[ESP+28] ; y0
	MOV		ESI,[ESP+32] ; x1
	MOV		EDI,[ESP+36] ; y1
	MOV		EBP,[ESP+40] ; col
	INT		0x40
	POP		EBX
	POP		EBP
	POP		ESI
	POP		EDI
	RET

draw_point_window:
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV		EDX,11
	MOV		EBX,[ESP+16]
	MOV		ESI,[ESP+20]
	MOV		EDI,[ESP+24]
	MOV		EAX,[ESP+28]
	INT		0x40
	POP		EBX
	POP		ESI
	POP		EDI
	RET

draw_line_window:
	PUSH	EDI
	PUSH	ESI
	PUSH	EBP
	PUSH	EBX
	MOV		EDX,13
	MOV		EBX,[ESP+20] ; win
	MOV		EAX,[ESP+24] ; x0
	MOV		ECX,[ESP+28] ; y0
	MOV		ESI,[ESP+32] ; x1
	MOV		EDI,[ESP+36] ; y1
	MOV		EBP,[ESP+40] ; col
	INT		0x40
	POP		EBX
	POP		EBP
	POP		ESI
	POP		EDI
	RET

refresh_window:
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV		EDX,12
	MOV		EBX,[ESP+16] ; win
	MOV		EAX,[ESP+20] ; x0
	MOV		ECX,[ESP+24] ; y0
	MOV		ESI,[ESP+28] ; x1
	MOV		EDI,[ESP+32] ; y1
	INT		0x40
	POP		EBX
	POP		ESI
	POP		EDI
	RET

get_key:
	MOV		EDX,15
	MOV		EAX,[ESP+4] ; mode
	INT		0x40
	RET

init_malloc:
	PUSH	EBX
	MOV		EDX,8
	MOV		EBX,[CS:0x0020]
	MOV		EAX,EBX
	ADD		EAX,32*1024
	MOV		ECX,[CS:0x0000]
	SUB		ECX,EAX
	INT		0x40
	POP		EBX
	RET

malloc:
	PUSH	EBX
	MOV		EDX,9
	MOV		EBX,[CS:0x0020]
	MOV		ECX,[ESP+8]
	INT		0x40
	POP		EBX
	RET

free:
	PUSH	EBX
	MOV		EDX,10
	MOV		EBX,[CS:0x0020]
	MOV		EAX,[ESP+ 8]
	MOV		ECX,[ESP+12]
	INT		0x40
	POP		EBX
	RET

alloc_timer:
	MOV		EDX,16
	INT		0x40
	RET

init_timer:
	PUSH	EBX
	MOV		EDX,17
	MOV		EBX,[ESP+8] ; timer
	MOV		EAX,[ESP+12] ; data
	INT		0x40
	POP		EBX
	RET

set_timeout:
	PUSH	EBX
	MOV		EDX,18
	MOV		EBX,[ESP+8] ; timer
	MOV		EAX,[ESP+12] ; time
	INT		0x40
	POP		EBX
	RET

api_freetimer:
	PUSH	EBX
	MOV		EDX,19
	MOV		EBX,[ESP+8] ; timer
	INT		0x40
	POP		EBX
	RET

beep: ; void api_beep(int tone)
	MOV		EDX,20
	MOV		EAX,[ESP+4] ; tone
	INT		0x40
	RET

api_end:
	MOV	 EDX,4
	INT		0x40
