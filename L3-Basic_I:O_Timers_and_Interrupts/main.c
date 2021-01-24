#include <stdio.h>

#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/slider_switches.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/HEX_displays.h"

int main(){
	HEX_flood_ASM(HEX4 | HEX5);
	while(1){
		write_LEDs_ASM(read_slider_switches_ASM());

		if(read_slider_switches_ASM() & 0x200){				//is 9th switch on?
			HEX_clear_ASM(HEX0 | HEX1 | HEX2 | HEX3);		//clear all displays
		}
		else
			HEX_write_ASM(read_PB_data_ASM() & 0xF, read_slider_switches_ASM() & 0xF);

	}

	return 0;
}

