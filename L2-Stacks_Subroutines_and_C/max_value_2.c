#include "address_map_arm.h"

/* This program demonstrates the use of parallel ports in the DE1-SoC Computer
 * It performs the following: 
 * 	1. displays the SW switch values on the red lights LEDR
 * 	2. displays a rotating pattern on the HEX displays
 * 	3. if a KEY[3..0] is pressed, uses the SW switches as the pattern
*/
extern int MAX_2(int x, int y);

int main(void)
{
	int a[5] = {5,4,3,2,1};
	int max_val = a[0];
	int i;
	for(i = 1; i < sizeof(a)/sizeof(a[0]); i++) {
		max_val = MAX_2(max_val, a[i]);	
	}
	return max_val;
}
