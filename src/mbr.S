%include "boot.inc"

SECTION MBR vstart=0x7c00
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov fs,ax
	mov sp,0x7c00
	mov ax,0xb800
	mov gs,ax

	mov ax,0x0600 ; ah=0x06 function number of int 0x10 (clear screen), al=0x00 (clear lines, 0x00 means all lines)
	mov bx,0x0700
	mov cx,0 ; (ch, cl) = (0,0)
	mov dx,0x184f ; (dh, dl) = (80, 25)

	int 0x10

	mov byte [gs:0x4],'M'
	mov byte [gs:0x5],0xA4

	mov byte [gs:0x6],'B'
	mov byte [gs:0x7],0xA4

	mov byte [gs:0x8],'R'
	mov byte [gs:0x9],0xA4

	mov eax,LOADER_START_SECTOR
	mov bx,LOADER_BASE_ADDR
	mov cx,1
	call rd_disk_m_16

	jmp LOADER_BASE_ADDR

rd_disk_m_16:
	mov esi,eax
	mov di,cx

	; sector count
	mov dx,0x1f2
	mov al,cl
	out dx,al

	mov eax,esi

	; LBA[7:0]
	mov dx,0x1f3
	out dx,al

	; LBA[15:8]
	mov dx,0x1f4
	mov cl,8
	shr eax,cl
	out dx,al

	; LBA[23:16]
	mov dx,0x1f5
	shr eax,cl
	out dx,al

	mov dx,0x1f6
	shr eax,cl
	and al,0x0f ; LBA[27:24]
	or al,0xe0
	out dx,al

	; disk command
	mov dx,0x1f7
	mov al,0x20
	out dx,al

.not_ready:
	nop
	in al,dx
	and al,0x88
	cmp al,0x08 ; data ready and disk not busy
	jnz .not_ready

	; count data length
	mov ax,di
	mov dx,256
	mul dx
	mov cx,ax

	; read data
	mov dx,0x1f0
.go_on_read:
	in ax,dx
	mov [bx],ax
	add bx,2
	loop .go_on_read
	ret

	times 510-($-$$) db 0
	db 0x55,0xaa
