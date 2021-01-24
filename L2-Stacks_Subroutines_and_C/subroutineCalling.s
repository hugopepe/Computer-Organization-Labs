			.text
			.global _start		
_start:
			LDR R0, N			//pass number of elements (by value) in R0
			LDR R1, =NUMBERS	//pass pointer to list (pointer to first element) in R1

			BL FINDMAX
			
			STR R0, RESULT

END:		B END				// infinite loop!

//No callee save is necessary because only the parameter registers (R0 - R3) are used in the subroutine
FINDMAX:	LDR R2, [R1]		//set R2 (max value) to the first number in the list
LOOP:		SUBS R0, R0, #1  	//decrement the loop counter
			BEQ DONE			//end loop if counter has reached 0
			ADD R1, R1, #4		//R1 points to next number in the list
			LDR R3, [R1]		//R1 holds the next number in the list
			CMP R2, R3			//check if it is greater than the maximum
			BGE LOOP			//if no, branch back to the loop
			MOV R2, R3			//if yes, update the current max
			B LOOP				//branch back to the loop

DONE:		MOV R0, R2
			BX LR

RESULT:		.word	0			//memory assigned for result location
N:			.word	7			//number of entries in the list
NUMBERS:	.word	4, 5, 3, 6	//the list data
			.word	1, 8, 2
