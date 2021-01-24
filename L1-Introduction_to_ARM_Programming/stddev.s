			.text
			.global _start	

_start:
			LDR R2, =N			//R2 points to the number of elements in the list
			ADD R3, R2, #4		//R3 points the first number
			LDR R0, [R3]		//R0 holds the first number
			LDR R2, [R2]		//R2 holds number of elements in the list

LOOP1:		SUBS R2, R2, #1  	//decrement the loop counter
			BEQ DONEMAX			//end loop if counter has reached 0
			ADD R3, R3, #4		//R3 points to next number in the list
			LDR R1, [R3]		//R1 holds the next number in the list
			CMP R0, R1			//check if it is greater than the maximum
			BGE LOOP1			//if no, branch back to the loop
			MOV R0, R1			//if yes, update the current max
			B LOOP1				//branch back to the loop

DONEMAX:	MOV R5, R0			//store the max number to R5
			STR R5, MAX

			LDR R2, =N			//R2 points to the number of elements in the list
			ADD R3, R2, #4		//R3 points the first number
			LDR R0, [R3]		//R0 holds the first number
			LDR R2, [R2]		//R2 holds number of elements in the list

LOOP2:		SUBS R2, R2, #1  	//decrement the loop counter
			BEQ DONEMIN			//end loop if counter has reached 0
			ADD R3, R3, #4		//R3 points to next number in the list
			LDR R1, [R3]		//R1 holds the next number in the list
			CMP R0, R1			//check if it is less than the minimum
			BLE LOOP2			//if no, branch back to the loop
			MOV R0, R1			//if yes, update the current max
			B LOOP2				//branch back to the loop

DONEMIN:	STR R0, MIN
			SUB R5, R5, R0		//store difference between maximum and minimum in R5

DIVIDE:		MOV R0, #4			//store the divider, 4, in R0
			MOV R1, R5			//store the dividend, the max - min value, in R1
			MOV R2, #0			//set remainder to zero
			MOV R3, #0 			//set quotient to zero
			MOV R10, #0			//set counter to zero

LOOP3:		AND R4, R1, #0x80000000	//get MSB of shifted dividend value (bring-down value)
			LSR R4, R4, #31     //shift MSB to LSB position
			LSL R2, R2, #1		//shift remainder 1 position to the left
			ADD R2, R2, R4		//add bring-down value to remainder
			LSL R1, R1, #1		//shift dividend by 1 to access next digit for the next loop 
			CMP R2, R0			//compare remainder + bring-down value with divider
			BLT TOOSMALL		//if the dividend cannot fit, branch
			ADD R3, R3, #1		//otherwise, set this digit of the quotient to 1
			SUB R2, R2, R0		//update remainder

TOOSMALL:	CMP R10, #31
			BEQ DONE			//if no more digits in dividend (counter R10 at end value), branch
			ADD R10, R10, #1	//otherwise, increment counter
			LSL R3, R3, #1		//shift quotient to next digit
			B LOOP3				//loop division step
			
DONE:		STR R3, QUOTIENT
			STR R2, REMAINDER
END:		B END				// infinite loop!

N:			.word	7			//number of entries in the list
NUMBERS:	.word	4, 5, 3, 6	//the list data
			.word	1, 8, 2
MAX:		.word	0			//max value in numbers
MIN:		.word	0			//min value in numbers
QUOTIENT:	.word	0			//standard deviation quotient
REMAINDER:	.word	0			//standard deviation remainder

