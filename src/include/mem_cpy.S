mem_cpy:
	cld ; clean direction: means esi,edi auto add size(byte word double)
	push ebp
	mov ebp,esp
	push ecx

	mov edi,[ebp+8]
	mov esi,[ebp+12]
	mov ecx,[ebp+16]
	rep movsb ; 1 byte each time

	pop ecx
	pop ebp
	ret
