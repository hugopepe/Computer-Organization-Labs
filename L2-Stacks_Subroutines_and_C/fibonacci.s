			.text
			.global _start
_start:
			LDR R0, N			//Set paramater R0 to input number
			BL FIB				//Call Fib

			STR R0, RESULT

END:		B END

FIB:		CMP R0, #2
			BLT DONE			//If n is less than 2, branch to DONE
								//otherwise, return Fib(n-1) + Fib(n-2)
			PUSH {LR}			//saving return address to restore LR after nested Fib calls
			SUB R0, R0, #1		//Set R0 to n-1
			PUSH {R0}			//Save R0 before getting Fib(n-1)
			BL FIB				//Set R0 to Fib(n-1)
			MOV R1, R0			//Set R1 to R0 (Fib(n-1))
			POP {R0}			//Restore R0 to n-1 by popping saved value from stack
			SUB R0, R0, #1		//Set R0 to n-2
			PUSH {R1}			//Save Fib(n-1) by pushing it onto the stack
			BL FIB				//Set R0 to Fib(n-2)
			POP {R1}			//Restore R1 to Fib(n-1) by popping saved value from stack
			ADD R0, R0, R1		//Set R0 to Fib(n-1) + Fib(n-2)
			
			POP {LR}			//restore return address by popping from stack
			BX LR				//return Fib value

DONE:		MOV R0, #1			//return 1
			BX LR


RESULT:		.word	0			//memory assigned for result location
N:			.word	7			//input number	
