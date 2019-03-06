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
	VECTOR 0x21,.NO_ERROR_CODE,keyboard_intr_handler
	VECTOR 0x22,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x23,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x24,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x25,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x26,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x27,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x28,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x29,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x2a,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x2b,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x2c,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x2d,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x2e,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x2f,.NO_ERROR_CODE,general_intr_handler

	VECTOR 0x30,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x31,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x32,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x33,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x34,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x35,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x36,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x37,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x38,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x39,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x3a,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x3b,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x3c,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x3d,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x3e,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x3f,.NO_ERROR_CODE,general_intr_handler

	VECTOR 0x40,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x41,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x42,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x43,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x44,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x45,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x46,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x47,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x48,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x49,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x4a,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x4b,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x4c,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x4d,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x4e,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x4f,.NO_ERROR_CODE,general_intr_handler

	VECTOR 0x50,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x51,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x52,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x53,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x54,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x55,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x56,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x57,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x58,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x59,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x5a,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x5b,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x5c,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x5d,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x5e,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x5f,.NO_ERROR_CODE,general_intr_handler

	VECTOR 0x60,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x61,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x62,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x63,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x64,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x65,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x66,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x67,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x68,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x69,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x6a,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x6b,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x6c,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x6d,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x6e,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x6f,.NO_ERROR_CODE,general_intr_handler

	VECTOR 0x70,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x71,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x72,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x73,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x74,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x75,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x76,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x77,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x78,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x79,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x7a,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x7b,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x7c,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x7d,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x7e,.NO_ERROR_CODE,general_intr_handler
	VECTOR 0x7f,.NO_ERROR_CODE,general_intr_handler

	; VECTOR 0x80,.NO_ERROR_CODE,syscall_handler
	SECTION .text
		extern syscall_table
		intr0x80entry:
			push ds
			push es
			push fs
			push gs
			pushad

			push 0x80
			push edx
			push ecx
			push ebx
			call [syscall_table + eax * 4]
			add esp,16

			; save return value (eax) into stack position of eax, then popad will pop into eax register
			mov [esp + 7 * 4], eax

			popad
			pop gs
			pop fs
			pop es
			pop ds

			iretd
	SECTION .data
		dd intr0x80entry
