#include <stdio.h>

#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/int_setup.h"
#include "./drivers/inc/ISRs.h"

int main() {

	int_setup(2, (int []){73,199});
	enable_PB_INT_ASM( PB0 | PB1 | PB2 );
	
	int ms1 = 0, ms2 = 0, s1 = 0, s2 = 0, m1 = 0, m2 = 0;
	HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);

	HPS_TIM_config_t stopwatchTimer;
	
	stopwatchTimer.tim = TIM0;
	stopwatchTimer.timeout = 10000;
	stopwatchTimer.LD_en = 1;
	stopwatchTimer.INT_en = 1;
	stopwatchTimer.enable = 0;

	HPS_TIM_config_ASM(&stopwatchTimer);

		

	//int running = 0;

	while(1){
		
		if(pb_int_flags){
			
			if(pb_int_flags & PB0) {
				//running = 1;
				stopwatchTimer.enable = 1;
				HPS_TIM_config_ASM(&stopwatchTimer);
			}
			if(pb_int_flags & PB1) {
				//running = 0;
				stopwatchTimer.enable = 0;
				HPS_TIM_config_ASM(&stopwatchTimer);
			}
			if(pb_int_flags & PB2){
				ms1 = 0;
				ms2 = 0;
				s1 = 0;
				s2 = 0;
				m1 = 0;
				m2 = 0;
				HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);
			}
			pb_int_flags = 0;
		}

		if(/*running*/ stopwatchTimer.enable && hps_tim0_int_flag){
			hps_tim0_int_flag = 0;
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
