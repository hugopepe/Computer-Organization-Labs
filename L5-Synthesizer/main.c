#include <stdio.h>
#include <math.h>

#include "./drivers/inc/vga.h"
#include "./drivers/inc/ISRs.h"
#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/audio.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/int_setup.h"
#include "./drivers/inc/wavetable.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/ps2_keyboard.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/slider_switches.h"

//frequencies
const char byteCodes[] = {0x1C, 0x1D, 0x1B, 0x24, 0x23, 0x2B, 0x2C, 0x34, 0x35, 0x33, 0x3C, 0x3B, 0x42};
const float baseFrequencies[] = {130.813, 138.592, 146.833, 155.564, 164.814, 174.614, 184.998, 195.998, 207.653, 220.000, 233.082, 246.942, 261.626};
const int numFrequencies = 13;

//octaves
const int maxOctave = 5;
const int minOctave = 1;
const int baseOctave = 3;
const char octaveCodes[] = {0x4E, 0x55};	//octave down, octave up

//amplitude
const int maxTableAmplitude = 0x800000;	//max amplitude in the sine wave table
const int maxAmplitude = 0xFFFFFF;			//max apmlitude the program permits (limited by max amplitude allowed by codec)

//volume
const int maxVolume = 50;					//maximum on arbitrary volume scale
const char volumeCodes[] = {0x01, 0x09};	//volume down, volume up codes

//display
const int llBound[] = {0, 10};
const int urBound[] = {319, 230};
const float displayScaler = 4.6875;


float signal(float f, int t) {
	float index = (f * t);
	int integer = (int) index;
	float remainder = index - (float) integer;
	integer = integer % 48000;

	float signal = (1 - remainder) * (float)sine[integer] + remainder * (float)sine[integer + 1];
	return signal;	//(int)
}


//Returns corresponding index of the charCode array for a given char. Returns -1 if does not contain it.
int getCharCodeIndex(char elem){
	int i;
	for(i = 0; i < numFrequencies; i++){
		if(byteCodes[i] == elem)
			return i;
	}
	return -1;
}


int main() {
	int_setup(1, (int []){199}); //enable ps2 interrupts

	//octaves
	int currentOctave = baseOctave;				//variable octave
	float frequencyMultiplier = 1;				//multiplier used to get base frequencies to frequencies of current octave
	
	//amplitude
	float finalSignal;	//final signal written to codec
	float ampPerVol = (float) maxAmplitude / ((float) maxTableAmplitude * (float) maxVolume); //scaler that converts a (sum of table signal amplitudes * current volume) to the final amplitude

	//volume
	int currentVolume = 50;	//variable volume

	//inputs and time
	unsigned int t = 0;
	char inputCode;
	char* pInputCode = &inputCode;
	int breakCode = 0;
	int keysPressed[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	//note keys pressed
	int waveUpdated = 0;

	//display
	float pixelPerAmp = (float)(urBound[1] - llBound[1]) / ((float) maxAmplitude * 6);
	unsigned int pixelt;
	int lastPixelY = -1;

	VGA_clear_charbuff_ASM();
	VGA_clear_pixelbuff_ASM();

	//write volume and octave displays
	VGA_write_char_ASM(0, 0, 'V');
	VGA_write_char_ASM(1, 0, 'O');
	VGA_write_char_ASM(2, 0, 'L');
	VGA_write_char_ASM(3, 0, 'U');
	VGA_write_char_ASM(4, 0, 'M');
	VGA_write_char_ASM(5, 0, 'E');
	VGA_write_char_ASM(6, 0, ':');
	VGA_write_char_ASM(7, 0, ' ');
	VGA_write_char_ASM(8, 0, currentVolume / 10 + '0');
	VGA_write_char_ASM(9, 0, (currentVolume % 10) + '0');

	VGA_write_char_ASM(0, 1, 'O');
	VGA_write_char_ASM(1, 1, 'C');
	VGA_write_char_ASM(2, 1, 'T');
	VGA_write_char_ASM(3, 1, 'A');
	VGA_write_char_ASM(4, 1, 'V');
	VGA_write_char_ASM(5, 1, 'E');
	VGA_write_char_ASM(6, 1, ':');
	VGA_write_char_ASM(7, 1, ' ');
	VGA_write_char_ASM(8, 1, currentOctave + '0');
	

	while(1) {
		//reading keyboard input
		if(read_ps2_data_ASM(pInputCode)){
			if(breakCode){	//code after break code
				int index = getCharCodeIndex(inputCode);
				if(index != -1){
					keysPressed[index] = 0;
					waveUpdated = 1;
				}
				breakCode = 0;
			}
			else if(inputCode == 0xF0){	//break code detected
				breakCode = 1;
			}
			else if(inputCode == octaveCodes[0] && currentOctave > minOctave){	//decrease octave
				frequencyMultiplier /= 2;
				currentOctave--;
				VGA_write_char_ASM(8, 1, currentOctave + '0');
				waveUpdated = 1;
			}
			else if(inputCode == octaveCodes[1] && currentOctave < maxOctave){	//increase octave
				frequencyMultiplier *= 2;
				currentOctave++;
				VGA_write_char_ASM(8, 1, currentOctave + '0');
				waveUpdated = 1;
			}
			else if(inputCode == volumeCodes[0] && currentVolume > 0){			//decrease volume
				currentVolume--;
				VGA_write_char_ASM(8, 0, currentVolume / 10 + '0');
				VGA_write_char_ASM(9, 0, (currentVolume % 10) + '0');
				waveUpdated = 1;
			}
			else if(inputCode == volumeCodes[1] && currentVolume < maxVolume){	//increase volume
				currentVolume++;
				VGA_write_char_ASM(8, 0, currentVolume / 10 + '0');
				VGA_write_char_ASM(9, 0, (currentVolume % 10) + '0');
				waveUpdated = 1;
			}
			else{	//another key detected
				int index = getCharCodeIndex(inputCode);
				if(index != -1 && !keysPressed[index]){	//valid byte code and pressed
					keysPressed[index] = 1;
					waveUpdated = 1;
				}
			}
		}
		
		//Writing to audio codec
		float signalSum = 0;
		int i;
		for(i = 0; i < numFrequencies; i++){
			if(keysPressed[i])	//bit i in keysPressed is 1?
				signalSum += signal(baseFrequencies[i] * frequencyMultiplier, t);
		}
			
		finalSignal = (int) (signalSum * ampPerVol * currentVolume);

		while(1) {
			if(audio_write_data_ASM(finalSignal, finalSignal)){
				t++;
				break;
			}
		}

		//Writing to VGA display
		if(waveUpdated){
			lastPixelY = -1;
			VGA_clear_pixelbuff_ASM();
			for(pixelt = 0; pixelt < (urBound[0] - llBound[0]); pixelt++){
				signalSum = 0;
				int j;
				for(j = 0; j < numFrequencies; j++){
					if(keysPressed[j]){	//bit j in keysPressed is 1?
						signalSum += signal(baseFrequencies[j] * frequencyMultiplier, (int) (pixelt * displayScaler));
						//printf("%f, %i\n", baseFrequencies[j] * frequencyMultiplier, keysPressed[j]);
					}
				}
				//printf("Total: %f, %u, %u\n", signalSum, (int) (pixelt * displayScaler), pixelt);
				finalSignal = (int) (signalSum * ampPerVol * currentVolume * pixelPerAmp);
				int pixelY = (int)(finalSignal + (urBound[1] + llBound[1]) / 2);
				VGA_draw_point_ASM(pixelt + llBound[0], pixelY, 0xAAAA);
				if(lastPixelY != -1){
					int k;
					if(pixelY > lastPixelY){
						for(k = pixelY - 1; k > lastPixelY; k--)
							VGA_draw_point_ASM(pixelt + llBound[0], k, 0xAAAA);
					}
					else if(pixelY < lastPixelY){
						for(k = lastPixelY - 1; k > pixelY; k--)
							VGA_draw_point_ASM(pixelt + llBound[0], k, 0xAAAA);
					}
				}
				lastPixelY = pixelY;
			}
			waveUpdated = 0;
		}
	}

	return 0;
}
