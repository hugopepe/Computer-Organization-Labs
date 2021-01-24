			.text
			.global _start
_start:
			LDR R0, N			//R0 is the length of the list N
			LDR R1, =LIST		//R1 points to the first list element

LOOP1:		MOV R2, R1			//R2 points to A[i]
			MOV R5, #1			//R5 is counter i, set to 1
			MOV R6, #1			//R6 is "sorted" boolean, set to true

LOOP2:		LDR R3, [R2], #4	//R3 holds A[i], R2 now points to A[i+1]
			LDR R4, [R2]		//R4 holds A[i+1]
			CMP R3, R4
			BLE SMALLER			//branch to smaller if A[i] <= A[i+1]

			MOV R6, #0			//set sorted to false
			STR R3, [R2], #-4
			STR R4, [R2], #4	//swap A[i] and A[i+1] in memory

SMALLER:	ADD R5, R5, #1		//increment i
			CMP R5, R0
			BLT LOOP2			//if i < N, finish going through the list

			CMP R6, #0
			BEQ LOOP1			//if list was not sorted this run, go through again

END:		B END

N:			.word 10
LIST:		.word -45, 3, 67, -4, 0, 2, 0, 3, 465, 1
