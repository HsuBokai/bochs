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
