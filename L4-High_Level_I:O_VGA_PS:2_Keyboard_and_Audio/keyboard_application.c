#include <stdio.h>

#include "./drivers/inc/VGA.h"
#include "./drivers/inc/ps2_keyboard.h"


int main(){
	VGA_clear_charbuff_ASM();
	int valid;
	int x = 0;
	int y = 0;
	char byte;
	char* pByte = &byte;
	while(1){
		valid = read_PS2_data_ASM(pByte);
		if(valid){
			if(y > 59){
				y = 0;
				VGA_clear_charbuff_ASM();
			}
			VGA_write_byte_ASM(x, y, byte);
			x += 3;
			if(x > 79){
				x = 0;
				y++;
			}
		}
	}
}
