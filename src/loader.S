%include "boot.inc"

SECTION loader vstart=LOADER_BASE_ADDR
	jmp loader_start

	;---------- gdt data structure ----------
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

%include "rd_disk_m_16.S"

loader_start:
	;---------- init stack pointer ----------
	mov sp,LOADER_BASE_ADDR

	;---------- print loader msg ----------
	mov bp,loadermsg
	mov cx,17
	mov ax,0x1301
	mov bx,0x001f
	mov dx,0x1800
	int 0x10

	;---------- load kernel image ----------
	mov eax,KERNEL_START_SECTOR
	mov bx,KERNEL_BIN_BASE_ADDR
	mov cx,KERNEL_SECTOR_LEN
	call rd_disk_m_16

	;---------- enable A20 ----------
	in al,0x92
	or al,0000_0010_b
	out 0x92,al

	;---------- load GDT ----------
	lgdt [gdt_ptr]

	;---------- set cr0 ----------
	mov eax,cr0
	or eax,0x00000001
	mov cr0,eax

	;---------- update pipeline ----------
	jmp dword SELECTOR_CODE:p_mode_start

[bits 32]
p_mode_start:
	;---------- init segment reg ----------
	mov ax,SELECTOR_DATA
	mov ds,ax
	mov es,ax
	mov ss,ax
	;---------- init stack pointer ----------
	mov esp,LOADER_STACK_TOP

	;---------- print msg in protection mode ----------
	mov ax,SELECTOR_VIDEO
	mov gs,ax

	mov byte [gs:160],'P'
	mov byte [gs:162],'r'
	mov byte [gs:164],'o'
	mov byte [gs:166],'t'
	mov byte [gs:168],'e'
	mov byte [gs:170],'c'
	mov byte [gs:172],'t'
	mov byte [gs:174],'i'
	mov byte [gs:176],'o'
	mov byte [gs:178],'n'


	;---------- setup pde and pte ----------
	call setup_page

	;---------- change VIDEO_DESC base address to virtual address ----------
	sgdt [gdt_ptr]
	mov ebx,[gdt_ptr+2]
	or dword [ebx+0x18+4],KERNEL_VIR_ADDR_MAP

	;---------- change GDT base address to virtual address ----------
	add dword [gdt_ptr+2],KERNEL_VIR_ADDR_MAP

	;---------- change stack pointer to virtual address ----------
	add esp,KERNEL_VIR_ADDR_MAP

	;---------- init Page Directory Base Register ----------
	mov eax,PAGE_DIR_TABLE_POS
	mov cr3,eax

	;---------- enable PG ----------
	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax

	;---------- change GDT base address to virtual address ----------
	lgdt [gdt_ptr]

	;---------- print msg in virtual address enable ----------
	mov byte [gs:320],'V'
	mov byte [gs:322],'i'
	mov byte [gs:324],'r'
	mov byte [gs:326],'t'
	mov byte [gs:328],'u'
	mov byte [gs:330],'a'
	mov byte [gs:332],'l'

	;---------- enter_kernel ----------
	call kernel_init
	mov esp,KERNEL_STACK_TOP
	mov eax,[KERNEL_BIN_BASE_ADDR+24]
	push eax
	ret ;---------- jmp KERNEL_ENTRY_POINT ----------

%include "setup_page.S"
%include "kernel_init.S"
