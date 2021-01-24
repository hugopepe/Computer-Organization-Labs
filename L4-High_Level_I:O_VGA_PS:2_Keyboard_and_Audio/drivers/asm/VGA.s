	.text

	.equ	PIXELBUFF_BASE, 0xC8000000
	.equ	CHARBUFF_BASE, 0xC9000000

	.global	VGA_clear_charbuff_ASM
	.global VGA_clear_pixelbuff_ASM
	.global VGA_write_char_ASM
	.global VGA_write_byte_ASM
	.global VGA_draw_point_ASM

VGA_clear_charbuff_ASM:
	PUSH {R4,R5}

	LDR R0, =CHARBUFF_BASE		//base address
	LDR R1, =CHARBUFF_WIDTH
	LDR R1, [R1]				//x size
	LDR R2, =CHARBUFF_HEIGHT
	LDR R2, [R2]				//y size
	MOV R4, #0x00				//0 constant to clear a word
	LDR R5, =0xFFFFFF80			//constant to reset x offset
YLOOP:
	MOV R3, R1					//reset x counter to max value
XLOOP:
	STRB R4, [R0], #1			//clear byte
	SUBS R3, R3, #1
	BGT XLOOP					//loop if not done clearing row
	AND R0, R0, R5				//reset x offset to 0
	ADD R0, R0, #0x80			//increment y by 1
	SUBS R2, R2, #1
	BGT YLOOP					//loop if not done clearing
	
	POP {R4,R5}
	BX LR

VGA_clear_pixelbuff_ASM:
	PUSH {R4,R5}

	LDR R0, =PIXELBUFF_BASE		//base address
	LDR R1, =PIXELBUFF_WIDTH
	LDR R1, [R1]				//x size
	LDR R2, =PIXELBUFF_HEIGHT
	LDR R2, [R2]				//y size
	MOV R4, #0x0000				//0 constant to clear a word
	LDR R5, =0xFFFFFC00			//constant to reset x offset
YLOOP_1:
	MOV R3, R1					//reset x counter to max value
XLOOP_1:
	STRH R4, [R0], #2			//clear word
	SUBS R3, R3, #1
	BGT XLOOP_1					//loop if not done clearing row
	AND R0, R0, R5				//reset x offset to 0
	ADD R0, R0, #0x400			//increment y by 1
	SUBS R2, R2, #1
	BGT YLOOP_1					//loop if not done clearing
	
	POP {R4,R5}
	BX LR


VGA_write_char_ASM:
	LDR R3, =CHARBUFF_BASE		//base address
	LSL R1, R1, #7				//fix y offset
	ADD R1, R1, R0				//calculate total offset
	STRB R2, [R3, R1]			//write char to [x,y]
	BX LR


VGA_write_byte_ASM:
	PUSH {R4,LR}
	//check if input parameters are in acceptable range
	LDR R3, =CHARBUFF_WIDTH
	LDR R3, [R3]
	LDR R4, =CHARBUFF_HEIGHT
	LDR R4, [R4]
	CMP R0, #0
	BLT END
	CMP R0, R3
	BGE END
	CMP R1, #0
	BLT END
	CMP R1, R4
	BGE END
	//get ASCII for upper half-byte
	LDR R3, =ASCII_CODES		//Set R3 to ASCII codes base address
	AND R4, R2, #0xF0			//get upper half-byte of last parameter
	LSR R4, R4, #4
	PUSH {R2}					//conserve parameter 3
	LDR R2, [R3, R4, LSL #2]	//get ASCII value
	AND R2, R2, #0xFF
	//write byte
	PUSH {R0,R1,R3}				//conserve parameters 1 & 2, ASCII base address
	BL VGA_write_char_ASM
	POP {R0,R1,R3}				
	POP {R2}
	//increment x (and y)
	LDR R4, =CHARBUFF_WIDTH
	LDR R4, [R4]				//get charbuff width
	SUB R4, R4, #1				//get charbuff width - 1
	CMP R0, R4
	BLT INCREMENT_X_BY_1
	//increment y by 1, set x to 0
	MOV R0, #0
	ADD R1, R1, #1
	LDR R4, =CHARBUFF_HEIGHT
	LDR R4, [R4]
	CMP R1, R4
	BEQ END
	B END_IF
INCREMENT_X_BY_1:
	ADD R0, R0, #1
END_IF:
	//get ASCII for lower half-byte
	AND R4, R2, #0xF			//get last half-byte of last parameter
	LDR R2, [R3, R4, LSL #2]	//get ASCII value
	AND R2, R2, #0xFF
	BL VGA_write_char_ASM
END:
	POP {R4,LR}
	BX LR


VGA_draw_point_ASM:
	LDR R3, =PIXELBUFF_BASE		//base address
	LSL R0, R0, #1				//fix x offset
	LSL R1, R1, #10				//fix y offset
	ADD R1, R1, R0				//calculate total offset
	STRH R2, [R3, R1]			//write char to [x,y]
	BX LR


CHARBUFF_WIDTH:		.word 80	//80 bytes
CHARBUFF_HEIGHT:	.word 60	//60 rows

PIXELBUFF_WIDTH:	.word 320	//320 halfwords
PIXELBUFF_HEIGHT:	.word 240	//240 rows

ASCII_CODES:		.word 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70
