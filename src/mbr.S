SECTION MBR vstart=0x7c00
	mov ax,cs
	mov es,ax
	mov ss,ax
	mov fs,ax
	mov sp,0x7c00
	mov ax,0xb800
	mov ds,ax

	mov ax,0x0600 ; ah=0x06 function number of int 0x10 (clear screen), al=0x00 (clear lines, 0x00 means all lines)
	mov bx,0x0700
	mov cx,0 ; (ch, cl) = (0,0)
	mov dx,0x184f ; (dh, dl) = (80, 25)

	int 0x10

	mov byte [0x4],'M'
	mov byte [0x5],0xA4

	mov byte [0x6],'B'
	mov byte [0x7],0xA4

	mov byte [0x8],'R'
	mov byte [0x9],0xA4

	jmp $

	times 510-($-$$) db 0
	db 0x55,0xaa
