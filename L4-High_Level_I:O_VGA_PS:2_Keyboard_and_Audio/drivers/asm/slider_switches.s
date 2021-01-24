	.text
	.equ	SW_BASE, 0xFF200040
	.global read_slider_switches_ASM

read_slider_switches_ASM:
	LDR R0, =SW_BASE
	LDR R0, [R0]
	BX LR

	.end
