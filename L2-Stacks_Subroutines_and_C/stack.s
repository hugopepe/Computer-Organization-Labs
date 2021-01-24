			.text
			.global _start

_start:		MOV R0, #1
			BL PUSH
			MOV R0, #2
			BL PUSH
			MOV R0, #3
			BL PUSH
			MOV R0, #4
			BL PUSH
			BL POP

END:		B END

PUSH:		STR	R0, [SP, #-4]!
			BX LR

POP:		LDMIA SP!, {R0-R2}
			BX LR

			.end
