#include <stdio.h>

#include "./drivers/inc/slider_switches.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/VGA.h"

void test_char(){
	int x,y;
	char c = 0;

	for(y = 0; y <= 59; y++){
		for(x = 0; x <= 79; x++)
			VGA_write_char_ASM(x, y, c++);
	}
}


void test_byte(){
	int x,y;
	char c = 0;

	for(y = 0; y <= 59; y++){
		for(x = 0; x <= 79; x+=3)
			VGA_write_byte_ASM(x, y, c++);
	}
}


void test_pixel(){
	int x,y;
	unsigned short colour = 0;

	for(y = 0; y <= 239; y++){
		for(x = 0; x <= 319; x++)
			VGA_draw_point_ASM(x, y, colour++);
	}
}


int main(){
	PB_clear_edgecp_ASM(PB0 | PB1 | PB2 | PB3);
	VGA_clear_charbuff_ASM();
	VGA_clear_pixelbuff_ASM();
	while(1){
		if(PB_edgecap_is_pressed_ASM(PB0)){
			if(read_slider_switches_ASM())
				test_byte();
			else
				test_char();
			PB_clear_edgecp_ASM(PB0);
		}
		else if(PB_edgecap_is_pressed_ASM(PB1)){
			test_pixel();
			PB_clear_edgecp_ASM(PB1);
		}
		else if(PB_edgecap_is_pressed_ASM(PB2)){
			VGA_clear_charbuff_ASM();
			PB_clear_edgecp_ASM(PB2);
		}
		else if(PB_edgecap_is_pressed_ASM(PB3)){
			VGA_clear_pixelbuff_ASM();
			PB_clear_edgecp_ASM(PB3);
		}
	}
	return 0;
}
