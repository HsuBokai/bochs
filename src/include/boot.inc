;---------- loader and kernel ----------
LOADER_BASE_ADDR	equ 0x900
LOADER_START_SECTOR	equ 0x2
LOADER_SECTOR_LEN	equ 4
LOADER_STACK_TOP	equ LOADER_BASE_ADDR

PAGE_DIR_TABLE_POS	equ 0x100000

KERNEL_BIN_BASE_ADDR	equ 0xc0001000
KERNEL_START_SECTOR	equ 0x9
KERNEL_SECTOR_LEN	equ 100
KERNEL_STACK_TOP	equ 0xc009f000
KERNEL_ENTRY_POINT	equ 0xc0001500

PT_NULL			equ 0

;---------- gdt attributes ----------
DESC_G_4K	equ (1b << 23)
DESC_D_32	equ (1b << 22)
DESC_L		equ (0b << 21)
DESC_AVL	equ (0b << 20)
DESC_P		equ (1b << 15)
DESC_DPL_0	equ (00b << 13)
DESC_DPL_1	equ (01b << 13)
DESC_DPL_2	equ (10b << 13)
DESC_DPL_3	equ (11b << 13)
DESC_S_SYS	equ (0b << 12)
DESC_S_NONSYS	equ (1b << 12)

; x=1, c=0, r=0, a=0
DESC_TYPE_CODE	equ (1000b << 8)

; x=0, e=0, w=1, a=0
DESC_TYPE_DATA	equ (0010b << 8)

;---------- selector attributes ----------
RPL0	equ 00b
RPL1	equ 01b
RPL2	equ 10b
RPL3	equ 11b
TI_GDT	equ (0b << 2)
TI_LDT	equ (1b << 2)

;---------- selector ----------
SELECTOR_CODE	equ (0x0001 << 3) + TI_GDT + RPL0
SELECTOR_DATA	equ (0x0002 << 3) + TI_GDT + RPL0
SELECTOR_VIDEO	equ (0x0003 << 3) + TI_GDT + RPL0

;---------- page attributes ------
PG_P		equ 1b
PG_RW_R		equ 00b
PG_RW_W		equ 10b
PG_US_S		equ 000b
PG_US_U		equ 100b
