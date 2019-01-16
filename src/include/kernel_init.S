kernel_init:
	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx

; for loop {
	mov dx,[KERNEL_BIN_BASE_ADDR+42]
	mov ebx,[KERNEL_BIN_BASE_ADDR+28]
	add ebx,KERNEL_BIN_BASE_ADDR
	mov cx,[KERNEL_BIN_BASE_ADDR+44]
.each_segment:
	cmp byte [ebx+0],PT_NULL
	je .PTNULL

	; call mem_cpy ( dst, src, size )
	push dword [ebx+16] ; size
	mov eax,[ebx+4]
	add eax,KERNEL_BIN_BASE_ADDR
	push eax ; src
	push dword [ebx+8] ; dst
	call mem_cpy
	add esp,12

.PTNULL:
	add ebx,edx
	loop .each_segment
; }

	ret

%include "mem_cpy.S"
