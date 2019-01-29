[bits 32]

global mem_cpy

SECTION .text
; mem_cpy(dst, src, count)
mem_cpy:
	cld ; clean direction: means esi,edi auto add size(byte word double)
	push ebp
	mov ebp,esp
	pushad

	mov edi,[ebp+8]
	mov esi,[ebp+12]
	mov ecx,[ebp+16]
	rep movsb ; 1 byte each time

	popad
	pop ebp
	ret
