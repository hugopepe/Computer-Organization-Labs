			.text
			.global _start


_start:		
			LDR R4, =ARRAY 			//R4 points to the first element of the array
			LDR R2, N				//R2 holds the number of elements in the list
			LDR R1, [R4]			//R1 holds the first element in the list

//sum all the numbers in the list

			
LOOP1:		ADD R4, R4, #4			//R4 points to the next element of the array
			SUBS R2, R2, #1			//decrement the loop counter
			BEQ  DIVISION			//end the loop if counter has reached zero
			LDR R3, [R4] 			//R3 holds the current element in the array
			ADD R1, R1, R3			//add the next number in the list to R1
			B LOOP1					//branch back to the loop

//now sum is in R1
//divide sum by the number of elements to compute average
//since the number of elements in the array can only be powers of 2, the division method is simplified
//shift the dividend and divisor right untill the divisor is 1, the result will be the shifted dividend

//R2 stores the divisor
		
DIVISION:	MOV R0, R1				//R0 holds the dividend
			LDR R1, N			//R1 holds the divisor
			
LOOP2:		CMP R1, #1				//Check if divisor is 1
			BEQ DONEAVG				//If the divisor is one, the result is in R0
			ASR R0, #1				//Shift dividend right (divide by two)
			ASR R1, #1				//Shift divisor right (divide by two)
			B LOOP2					//return to top of the loop
			

//subtract all array elements by the average

DONEAVG:	LDR R2, N				//R2 holds the number of elements in the array
			MOV R1, R0				//R1 holds the average of the array
			LDR R4, =ARRAY			//R4 points toward the first element of the array

LOOP3:		LDR R0, [R4]			//R0 holds the current element of the array
			SUB R0, R0, R1			//subtract average to current element of the array 
			STR R0, [R4], #4		//stores the centered element to location of corresponding element in memory changes R4 to point to next element
			SUBS R2, #1				//decrement the loop counter
			BEQ END 				//if counter is zero, branch to end
			B LOOP3					//return to top of the loop

END:		B END 					//infinite loop


N:			.word 	8				//number of entries in the array
ARRAY:		.word	-6, -5, -4, -3, -1, 0, 1, 2  	//array elements
 


