[bits 32]

global intr_entry_table

%macro VECTOR 3
	SECTION .text
	extern %3
	intr%1entry:
		push ds
		push es
		push fs
		push gs
		pushad

		mov al,0x20
		out 0xa0,al
		out 0x20,al

		push %1
		call %3
		add esp,4

		popad
		pop gs
		pop fs
		pop es
		pop ds

		jmp %2
	.SKIP_ERROR_CODE:
		add esp,4
	.NO_ERROR_CODE:

		iretd

	SECTION .data
		dd intr%1entry
%endmacro

SECTION .data
intr_entry_table:
	VECTOR 0x00,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x01,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x02,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x03,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x04,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x05,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x06,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x07,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x08,.SKIP_ERROR_CODE,general_intr_handler
	VECTOR 0x09,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x0a,.SKIP_ERROR_CODE,general_intr_handler
	VECTOR 0x0b,.SKIP_ERROR_CODE,general_intr_handler
	VECTOR 0x0c,.SKIP_ERROR_CODE,general_intr_handler
	VECTOR 0x0d,.SKIP_ERROR_CODE,general_intr_handler
	VECTOR 0x0e,.SKIP_ERROR_CODE,general_intr_handler
	VECTOR 0x0f,.NO_ERROR_CODE,general_intr_handler

	VECTOR 0x10,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x11,.SKIP_ERROR_CODE,general_intr_handler
	VECTOR 0x12,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x13,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x14,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x15,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x16,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x17,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x18,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x19,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x1a,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x1b,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x1c,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x1d,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x1e,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x1f,.NO_ERROR_CODE,general_intr_handler

	VECTOR 0x20,.NO_ERROR_CODE,timer_intr_handler
