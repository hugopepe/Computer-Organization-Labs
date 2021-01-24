#include <stdio.h>

#include "./drivers/inc/audio.h"

int main(){
	const int frequency = 100;			//cycles per second
	const int samplingRate = 48000;		//samples per second
	const int highValue = 0x00FFFFFF;
	const int lowValue = 0x00000000;
	
	int samplesPerCycle = samplingRate / frequency;
	
	int sampleCounter = 0;

	while(1){
		if(sampleCounter < samplesPerCycle / 2){
			if(write_audio_data_ASM(highValue)){
				sampleCounter++;
			}
		}
		else{
			if(write_audio_data_ASM(lowValue)){
				sampleCounter++;
				if(sampleCounter >= samplesPerCycle)
					sampleCounter = 0;
			}
		}
	}

}
