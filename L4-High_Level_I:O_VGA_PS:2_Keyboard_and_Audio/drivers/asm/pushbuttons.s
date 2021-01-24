	.text
	.equ	BUTTON_DATA_BASE, 0xFF200050
	.equ	BUTTON_MASK_BASE, 0xFF200058
	.equ	BUTTON_EDGE_BASE, 0xFF20005C
	.global read_PB_data_ASM
	.global PB_data_is_pressed_ASM
	.global read_PB_edgecap_ASM
	.global PB_edgecap_is_pressed_ASM
	.global PB_clear_edgecp_ASM
	.global enable_PB_INT_ASM
	.global disable_PB_INT_ASM

read_PB_data_ASM:
	LDR R0, =BUTTON_DATA_BASE
	LDR R0, [R0]
	BX LR


PB_data_is_pressed_ASM:
	LDR R1, =BUTTON_DATA_BASE
	LDR R1, [R1]
	AND R1, R1, R0
	CMP R1, R0
	BEQ IS_PRESSED1
	MOV R0, #0
	BX LR
IS_PRESSED1:
	MOV R0, #1
	BX LR


read_PB_edgecap_ASM:
	LDR R0, =BUTTON_EDGE_BASE
	LDR R0, [R0]
	BX LR


PB_edgecap_is_pressed_ASM:
	LDR R1, =BUTTON_EDGE_BASE
	LDR R1, [R1]
	AND R1, R1, R0
	CMP R1, R0
	BEQ IS_PRESSED2
	MOV R0, #0
	BX LR
IS_PRESSED2:
	MOV R0, #1
	BX LR


PB_clear_edgecp_ASM:

	LDR R1, =BUTTON_EDGE_BASE
	STR R0, [R1]
	BX LR

	
enable_PB_INT_ASM:
	LDR R1, =BUTTON_MASK_BASE
	LDR R2, [R1]
	ORR R0, R0, R2
	STR R0, [R1]
	BX LR


disable_PB_INT_ASM:
	LDR R1, =BUTTON_MASK_BASE
	LDR R2, [R1]
	MVN R0, R0
	AND R0, R0, R2
	STR R0, [R1]
	BX LR

	.end
