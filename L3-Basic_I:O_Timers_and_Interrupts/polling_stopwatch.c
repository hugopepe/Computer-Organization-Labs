#include <stdio.h>

#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/HPS_TIM.h"

int main() {
	
	int ms1 = 0, ms2 = 0, s1 = 0, s2 = 0, m1 = 0, m2 = 0;
	HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);

	HPS_TIM_config_t stopwatchTimer;
	
	stopwatchTimer.tim = TIM0;
	stopwatchTimer.timeout = 10000;
	stopwatchTimer.LD_en = 1;
	stopwatchTimer.INT_en = 1;
	stopwatchTimer.enable = 0; //initially 1

	HPS_TIM_config_ASM(&stopwatchTimer);


	HPS_TIM_config_t edgeTimer;
	
	edgeTimer.tim = TIM1;
	edgeTimer.timeout = 5000;
	edgeTimer.LD_en = 1;
	edgeTimer.INT_en = 1;
	edgeTimer.enable = 1;

	HPS_TIM_config_ASM(&edgeTimer);

	//int running = 0;

	while(1){
		
		if(HPS_TIM_read_INT_ASM(TIM1)){
			HPS_TIM_clear_INT_ASM(TIM1);
			if(PB_edgecap_is_pressed_ASM(PB0)){
				//running = 1;
				stopwatchTimer.enable = 1;
				HPS_TIM_config_ASM(&stopwatchTimer);
			}
			if(PB_edgecap_is_pressed_ASM(PB1)){
				//running = 0;
				stopwatchTimer.enable = 0;
				HPS_TIM_config_ASM(&stopwatchTimer);
			}
			if(PB_edgecap_is_pressed_ASM(PB2)){
				ms1 = 0;
				ms2 = 0;
				s1 = 0;
				s2 = 0;
				m1 = 0;
				m2 = 0;
				HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);
			}
			PB_clear_edgecp_ASM(PB0 | PB1 | PB2);
		}

		if(/*running*/ stopwatchTimer.enable && HPS_TIM_read_INT_ASM(TIM0)){
			HPS_TIM_clear_INT_ASM(TIM0);
			if(++ms1 == 10){
				ms1 = 0;
				if(++ms2 == 10){
					ms2 = 0;
					if(++s1 == 10){
						s1 = 0;
						if(++s2 == 6){
							s2 = 0;
							if(++m1 == 10){
								m1 = 0;
								m2++;
								HEX_write_ASM(HEX5, m2);
							}
							HEX_write_ASM(HEX4, m1);
						}
						HEX_write_ASM(HEX3, s2);
					}
					HEX_write_ASM(HEX2, s1);
				}
				HEX_write_ASM(HEX1, ms2);
			}
			HEX_write_ASM(HEX0, ms1);
		}
	}

}
