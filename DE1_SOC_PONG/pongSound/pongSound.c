/*
 * pongSound.c
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

#include "pongSound.h"


volatile unsigned int SoundOn = 1;

// Variables
volatile int sound = 1;
int freq = 1000;
float duration = 1;

//Pointers
volatile unsigned char* fifospace_ptr;
volatile unsigned int*  audio_left_ptr;
volatile unsigned int*  audio_right_ptr;
volatile unsigned int* HPS_timer_ptr = (unsigned int *) 0xFFC08000;

//Variables
//Phase Accumulator
double phase = 0.0;  // Phase accumulator
double inc   = 0.0;  // Phase increment
double baseampl  = 0.0;  // Tone amplitude (i.e. volume)
signed int audio_sample = 0;


void timerISR(HPSIRQSource interruptID, bool isInit, void* initParams) {
    if (!isInit) {
        volatile unsigned int * HPS_timer_ptr = (unsigned int *) 0xFFC08000;
        int temp = 0;
        //Clear the Timer Interrupt Flag
        //By reading timer end of interrupt register
        temp = HPS_timer_ptr[3];
        // Turn sound off
        sound = 0;
    }
    //Reset watchdog.
    HPS_ResetWatchdog();
}



void pongSound_Init(){
	//Initialise the Audio Codec.
	WM8731_initialise(0xFF203040);
}

void Sound(int _freq, float _duration){
	//Duration in milliseconds
	if (SoundOn){
		freq = _freq; duration = _duration;
		sound = 1;


		HPS_timer_ptr[2] = 0; // write to control register to stop timer
		// Set the timer period
		HPS_timer_ptr[0] = duration * 100000; // period = 1/(100 MHz) x (100 x 10^6) = 1 sec
		// Write to control register to start timer, with interrupts
		HPS_timer_ptr[2] = 0x03; // mode = 1, enable = 1
		// Register interrupt handler for timer
		HPS_IRQ_registerHandler(IRQ_TIMER_L4SP_0, timerISR);
		HPS_ResetWatchdog();

		//Clear both FIFOs
		WM8731_clearFIFO(true,true);
		//Grab the FIFO Space and Audio Channel Pointers
		fifospace_ptr = WM8731_getFIFOSpacePtr();
		audio_left_ptr = WM8731_getLeftFIFOPtr();
		audio_right_ptr = WM8731_getRightFIFOPtr();
		//Initialise Phase Accumulator
		inc   = ((float) freq) * 360.0 / F_SAMPLE;  // Calculate the phase increment based on desired frequency - e.g. 440Hz
		baseampl  = 8388608.0;               // Pick desired amplitude (e.g. 2^23). WARNING: If too high = deafening!
		phase = 0.0;
		// Primary function while loop
		while(sound == 1){
			//Always check the FIFO Space before writing or reading left/right channel pointers
			if ((fifospace_ptr[2] > 0) && (fifospace_ptr[3] > 0)) {
				//If there is space in the write FIFO for both channels:
				//Increment the phase
				phase = phase + inc;
				//Ensure phase is wrapped to range 0 to 2Pi (range of sin function)
				while (phase >= 360.0) {
					phase = phase - 360.0;
				}
				// Calculate next sample of the output tone.
				audio_sample = (signed int)( (1<<VOLUME)*baseampl * lookupSin((int) phase) );
				// Output tone to left and right channels.
				*audio_left_ptr = audio_sample;
				*audio_right_ptr = audio_sample;
			}
		}
		// Disable timer ISR
		HPS_timer_ptr[2] = 0x02; // mode = 1, enable = 0
		//Finally reset the watchdog.
		HPS_ResetWatchdog();
	}
}

void enableSound(unsigned int _onoff){
	SoundOn = _onoff;
}

void toggleSound(){
	if (SoundOn == 1) { SoundOn = 0; } else SoundOn = 1;
}


// Fast sin lookup
float lookupSin(unsigned int degree){
	// Angles found with matlab, x10^5 and rounded
	static int lookup[91] = { 0, 1745, 3490, 5234, 6976, 8716, 10453, 12187, 13917,
						15643, 17365, 19081, 20791, 22495, 24192, 25882, 27564,
						29237, 30902, 32557, 34202, 35837, 37461, 39073, 40674,
						42262, 43837, 45399, 46947, 48481, 50000, 51504, 52992,
						54464, 55919, 57358, 58779, 60182, 61566, 62932, 64279,
						65606, 66913, 68200, 69466, 70711, 71934, 73135, 74314,
						75471, 76604, 77715, 78801, 79864, 80902, 81915, 82904,
						83867, 84805, 85717, 86603, 87462, 88295, 89101, 89879,
						90631, 91355, 92050, 92718, 93358, 93969, 94552, 95106,
						95630, 96126, 96593, 97030, 97437, 97815, 98163, 98481,
						98769, 99027, 99255, 99452, 99619, 99756, 99863, 99939,
						99985, 100000 };
	if (degree <= 90){
		return (float) lookup[degree]/100000;
	}
	else if ((degree > 90) && (degree <= 180)){
		return (float) lookup[180-degree]/100000;
	}
	else if ((degree > 180) && (degree <= 270)){
		return (float) -lookup[degree-180]/100000;
	}
	else if ((degree > 270) && (degree <= 360)){
		return (float) -lookup[360-degree]/100000;
	} else return 0;
}
