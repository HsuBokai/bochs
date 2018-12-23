%include "boot.inc"

SECTION loader vstart=LOADER_BASE_ADDR
	jmp loader_start

	; gdt data structure
	GDT_BASE:	dd	0x00000000
			dd	0x00000000
	CODE_DESC:	dd	0x0000FFFF
			dd	(0x00 << 24) + \
				DESC_G_4K + \
				DESC_D_32 + \
				DESC_L + \
				DESC_AVL + \
				(0xF << 16) + \
				DESC_P + \
				DESC_DPL_0 + \
				DESC_S_NONSYS + \
				DESC_TYPE_CODE + \
				0x00
	DATA_STACK_DESC:	dd	0x0000FFFF
				dd	(0x00 << 24) + \
					DESC_G_4K + \
					DESC_D_32 + \
					DESC_L + \
					DESC_AVL + \
					(0xF << 16) + \
					DESC_P + \
					DESC_DPL_0 + \
					DESC_S_NONSYS + \
					DESC_TYPE_DATA + \
					0x00
	VIDEO_DESC:	dd	0x80000007 ; limit=(0xbffff-0xb8000)/4k=0x7
			dd	(0x00 << 24) + \
				DESC_G_4K + \
				DESC_D_32 + \
				DESC_L + \
				DESC_AVL + \
				(0x0 << 16) + \
				DESC_P + \
				DESC_DPL_0 + \
				DESC_S_NONSYS + \
				DESC_TYPE_DATA + \
				0x0b

	GDT_SIZE	equ	$-GDT_BASE
	GDT_LIMIT	equ	GDT_SIZE-1

	times 60 dq 0 ; reserve 60 gdt

	gdt_ptr:	dw GDT_LIMIT
			dd GDT_BASE

	loadermsg db '2 loader in real.'

loader_start:
	mov sp,LOADER_BASE_ADDR
	mov bp,loadermsg
	mov cx,17
	mov ax,0x1301
	mov bx,0x001f
	mov dx,0x1800
	int 0x10

	in al,0x92
	or al,0000_0010_b
	out 0x92,al

	lgdt [gdt_ptr]

	mov eax,cr0
	or eax,0x00000001
	mov cr0,eax

	jmp dword SELECTOR_CODE:p_mode_start

[bits 32]
p_mode_start:
	mov ax,SELECTOR_DATA
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov esp,LOADER_STACK_TOP
	mov ax,SELECTOR_VIDEO
	mov gs,ax

	mov byte [gs:160],'P'

	jmp $
