[bits 32]

global mem_cpy

SECTION .text
; extern void mem_cpy(int8_t *dst, int8_t *src, uint32_t bytes_count);
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
