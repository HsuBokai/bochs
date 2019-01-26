#ifndef __GLOBAL_H
#define __GLOBAL_H

//---------- KERNEL SELECTOR ----------
#define DPL0 0
#define DPL1 1
#define DPL2 2
#define DPL3 3

#define TI_GDT 0
#define TI_LDT 1

#define SELECTOR_K_CODE ((1<<3) + (TI_GDT << 2) + DPL0)
#define SELECTOR_K_DATA ((2<<3) + (TI_GDT << 2) + DPL0)
#define SELECTOR_K_STACK SELECTOR_K_DATA
#define SELECTOR_K_VIDEO ((3<<3) + (TI_GDT << 2) + DPL0)

//---------- IDT DESCRIPTOR ATTRIBUTES ----------
#define IDT_DESC_P 1
#define IDT_DESC_DPL0 0
#define IDT_DESC_DPL3 3
#define IDT_DESC_32_TYPE 0xE
#define IDT_DESC_16_TYPE 0x6

#define IDT_DESC_ATTR_DPL0 \
	((IDT_DESC_P << 7) + (IDT_DESC_DPL0 << 5) + IDT_DESC_32_TYPE)
#define IDT_DESC_ATTR_DPL3 \
	((IDT_DESC_P << 7) + (IDT_DESC_DPL3 << 5) + IDT_DESC_32_TYPE)

#endif /* __GLOBAL_H */
