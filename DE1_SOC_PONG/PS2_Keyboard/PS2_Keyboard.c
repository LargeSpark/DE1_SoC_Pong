#include "./PS2_Keyboard.h"
#include "../HPS_IRQ/HPS_IRQ.h"
#include "../pongEngine/pongEngine.h"
#include "../pongDisplay/pongDisplay.h"


#define batSpeed 10
// http://www-ug.eecg.utoronto.ca/msl/nios_devices_SoC/datasheets/PS2%20Keyboard%20Protocol.htm

/* Notes: Add option for key input
 * 		  Add audio output
 * 		  Integrate inputs with bat motions
 * 		  */



/* To set function running, simply include PS2_Keyboard.h and then call keyboardInitialise */
/* Requires scatter file change to FPGARomRamIRQ.scat */


volatile unsigned int *PS2_DATA = (unsigned int *) 0xFF200100; 		// PS2 base address
volatile unsigned int *PS2_CONTROL = (unsigned int *) 0xFF200104;	// PS2 control address
volatile unsigned int * KEY_ptr       = (unsigned int *) 0xFF200050; // Keys

volatile unsigned int keyBuffer[5] = {0,0,0};

bool keyboardIsInit = false;

volatile signed int paddleDirs[2] = {-1,-1};

// Size of FIFO = 256byte, 8 bit
// https://www.avrfreaks.net/sites/default/files/PS2%20Keyboard.pdf

void keyboardISR(HPSIRQSource interruptID, bool isInit, void* initParams){
	if (!isInit){
		*PS2_CONTROL |= 0x0;
		inputKeys();
		//*PS2_DATA |= 0x00;
		*PS2_CONTROL |= 0x1;
		emptyFIFO();
	}
	HPS_ResetWatchdog();
}

/* Requires fixing */
void pushbuttonISR(HPSIRQSource interruptID, bool isInit, void* initParams){
}

void keyboardInitialise(void) {
	if (keyboardIsInit == false){
		// Initialise interrupt
		HPS_IRQ_initialise(NULL);
		HPS_ResetWatchdog();
		HPS_IRQ_registerHandler(IRQ_LSC_PS2_PRIMARY, keyboardISR);
		HPS_ResetWatchdog();
		//HPS_IRQ_registerHandler(IRQ_LSC_KEYS, pushbuttonISR);
		//KEY_ptr[2] = 0xF; // Set key interrupts
		*PS2_CONTROL |= 0x1; // Set interrupt on PS2

		// Flag keyboard as initialised
		keyboardIsInit = true;
		HPS_ResetWatchdog();
	}
}

char PS2Scan( void ) {
	int keyboard_data, RVALID;
	char inputKey;

	keyboard_data = *(PS2_DATA);		// Read Data register
	RVALID = keyboard_data & 0x8000; 	// Extract RVALID
	if (RVALID){
		inputKey = keyboard_data & 0xFF; HPS_ResetWatchdog();
	} else { inputKey = 0; }//Handle any odd errors
	HPS_ResetWatchdog();
	return inputKey;
}

void inputKeys( void ) {
	char key = PS2Scan();
	keyBuffer[0] = keyBuffer[1];
	keyBuffer[1] = keyBuffer[2];
	keyBuffer[2] = PS2Scan();

	if (key == _W) 	{ keyBuffer[2] = 1; paddleDirs[0] = UP;  pongEngine_paddleMove(1, DOWN, batSpeed);} else if (key == _S) 		{ keyBuffer[2] = 2; paddleDirs[0] = DOWN; pongEngine_paddleMove(1, UP, batSpeed);} else { paddleDirs[0] = -1; }
	if (key == _UP) { keyBuffer[2] = 3; paddleDirs[1] = UP;  pongEngine_paddleMove(2, DOWN, batSpeed);} else if (key == _DOWN) 		{ keyBuffer[2] = 4; paddleDirs[1] = DOWN; pongEngine_paddleMove(2, UP, batSpeed);} else { paddleDirs[1] = -1; }
	if ((key == 0xF0) || (key == 0xE0)) keyBuffer[2] = 0; 							// Ignore make/break signals
	if ((keyBuffer[2] == keyBuffer[0]) && (keyBuffer[1] == 0)) keyBuffer[2] = 0; 	// Avoid doubling motions on single press due to make/break
	HPS_ResetWatchdog();
	// Put a state machine here which acts on different values of keyBuffer[2]
}

signed int paddleDir(int paddle){
	if (paddle == 1){
		return paddleDirs[0];
	} else {
		return paddleDirs[1];
	}
}

void emptyFIFO( void ){
	int RAVAIL = *(PS2_DATA) & 0xFFFF0000;
	char temp;
	while (RAVAIL > 0x1000){
		temp = *(PS2_DATA) & 0xFF;
		RAVAIL = *(PS2_DATA) & 0xFFFF0000;
	}
}
