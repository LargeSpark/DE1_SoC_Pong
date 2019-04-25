/* To set function running, simply include PS2_Keyboard.h and then call inputsInitialise */
/* Requires scatter file change to an IRQ ready file */
// http://www-ug.eecg.utoronto.ca/msl/nios_devices_SoC/datasheets/PS2%20Keyboard%20Protocol.htm

#ifndef PONGINPUTS_PONGINPUTS_H_
#define PONGINPUTS_PONGINPUTS_H_

#include <stdbool.h>
#include "../pongEngine/pongEngine.h"
#include "../pongDisplay/pongDisplay.h"
#include "../pongSound/pongSound.h"
#include "../pongScreens/pongScreens.h"
#include <string.h>
#include "../HPS_IRQ/HPS_IRQ.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include <stdio.h>
#include <stdlib.h>


// Keyboard scancodes for scan set 2
// From https://techdocs.altium.com/display/FPGA/PS2+Keyboard+Scan+Codes
#define _ESC 		0x76
#define _RETURN		0x5A
#define _BKSPACE 	0x66
#define _W			0x1D
#define _A			0x1C
#define _S			0x1B
#define _D			0x23
#define _LEFT		0x6B
#define _RIGHT		0x74
#define _UP			0x75
#define _DOWN		0x72

// Definitions for modes
#define MENUS 		0
#define GAME  		1
#define GAME_AI 	2

// Set paddle speeds
#define keySpeed  20
#define keyBSpeed 10


/* These are all pretty much used exclusively by this library. Interrupts handle everything and pass to InputControl. */

// Initialise the inputs
void inputsInitialise(void);

// Get raw keyboard scan codes, including make/break codes
char PS2Scan(void);

// Convert keyboard input to usable inputs and pass to Input()
void inputKeyboard( void );

// Clears FIFO on PS/2
void emptyFIFO( void );

// Takes input from keyboard/keys and acts on it depending on screen mode
void Input(unsigned int key, unsigned int speed);

// Set screen mode. Add switching between menus?
void setInputMode(unsigned int _mode);

// Get screen mode
int getInputMode( void );

// Enable/Disable inputs
void enableInputs(int enable);

#endif /* PONGINPUTS_PONGINPUTS_H_ */
