/*
 * pongSound.h
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

/*  		Warning - sounds are BLOCKING 		*/

#include "../DE1SoC_WM8731/DE1SoC_WM8731.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_IRQ/HPS_IRQ.h"

#ifndef _PONGSOUND_PONGSOUND_H
#define _PONGSOUND_PONGSOUND_H

//Define some useful constants
#define F_SAMPLE 48000.0        // Sampling rate of WM8731 Codec (Do not change)
#define VOLUME 	 4				// Don't put this too much higher with headphones in

// Initialise driver
void pongSound_Init( void );

// Play a sound of frequency _freq Hz for _duration milliseconds
void Sound(int _freq, float _duration);

// Enable (1) or disable (0) sounds
void enableSound(unsigned int _onoff);

// Toggle sound
void toggleSound( void );

// Fast sin implementation using lookup table(0-90*), characteristics of sin and fixed point(ish) storage
float lookupSin(unsigned int degree);

// Should probably add volume control too




#endif /* PONGSOUND_PONGSOUND_H */
