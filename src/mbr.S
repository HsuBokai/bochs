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
	mov cx,LOADER_SECTOR_LEN
	call rd_disk_m_16

	jmp LOADER_BASE_ADDR

%include "rd_disk_m_16.S"

	times 510-($-$$) db 0
	db 0x55,0xaa
