#include "pongInputs.h"
//#include "../HPS_IRQ/HPS_IRQ.h"
//#include "../pongEngine/pongEngine.h"
//#include "../pongDisplay/pongDisplay.h"


/* Notes: Add audio output
 * 		  */

/* To set function running, simply include PS2_Keyboard.h and then call inputsInitialise */
/* Requires scatter file change to FPGARomRamIRQ.scat */


volatile unsigned int *PS2_DATA = (unsigned int *) 0xFF200100; 		// PS2 base address
volatile unsigned int *PS2_CONTROL = (unsigned int *) 0xFF200104;	// PS2 control address
volatile unsigned int *KEY_ptr       = (unsigned int *) 0xFF200050; // Keys

volatile unsigned int keyBuffer[3] = {0,0,0};

bool inputsIsInit = false;

volatile unsigned mode = GAME;

// https://www.avrfreaks.net/sites/default/files/PS2%20Keyboard.pdf


/* IRQ handlers */
void keyboardISR(HPSIRQSource interruptID, bool isInit, void* initParams){
	if (!isInit){
		*PS2_CONTROL |= 0x0;
		inputKeyboard();
		//*PS2_DATA |= 0x00;
		*PS2_CONTROL |= 0x1;
		emptyFIFO();
	}
	HPS_ResetWatchdog();
}

/* Requires fixing */
void pushbuttonISR(HPSIRQSource interruptID, bool isInit, void* initParams) {
    if (!isInit) {
        unsigned int press;
        //Read the Push-button interrupt register
        press = KEY_ptr[3];
        //Then clear the interrupt flag by writing the value back
        KEY_ptr[3] = press;

        if (press == 1){
        	Input(3, keySpeed);
        }
        else if (press == 2){
        	Input(4, keySpeed);
        }
        else if (press == 4){
        	Input(1, keySpeed);
        }
        else if (press == 8){
        	Input(2, keySpeed);
        }
    }
    //Reset watchdog.
    HPS_ResetWatchdog();
}


/* Actual functions */

void inputsInitialise(void) {
	if (!inputsIsInit){
		// Initialise interrupts
		HPS_IRQ_initialise(NULL);
		HPS_ResetWatchdog();

		// Register IRQs
		HPS_IRQ_registerHandler(IRQ_LSC_PS2_PRIMARY, keyboardISR);
		HPS_ResetWatchdog();
		HPS_IRQ_registerHandler(IRQ_LSC_KEYS, pushbuttonISR);
		HPS_ResetWatchdog();

		// Enable interrupts
		KEY_ptr[2] = 0xF; 		// Set key interrupts
		*PS2_CONTROL |= 0x1; 	// Set interrupt on PS2

		// Flag keyboard as initialised
		inputsIsInit = true;
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

void inputKeyboard( void ) {
	char key = PS2Scan();
	keyBuffer[0] = keyBuffer[1];
	keyBuffer[1] = keyBuffer[2];
	keyBuffer[2] = PS2Scan();

	if (key == _W) 				{
		keyBuffer[2] = 1; Input(1, keyBSpeed);
	} else if (key == _S)		{
		keyBuffer[2] = 2; Input(2, keyBSpeed);
	} else if (key == _UP) 		{
		keyBuffer[2] = 3; Input(3, keyBSpeed);
	} else if (key == _DOWN) 	{
		keyBuffer[2] = 4; Input(4, keyBSpeed);
	}

	// Ignore make/break signals
	if ((key == 0xF0) || (key == 0xE0)) keyBuffer[2] = 0;

	// Avoid doubling motions on single press due to make/break
	if ((keyBuffer[2] == keyBuffer[0]) && (keyBuffer[1] == 0)) keyBuffer[2] = 0;

	HPS_ResetWatchdog();
}

void emptyFIFO( void ){
	int RAVAIL = *(PS2_DATA) & 0xFFFF0000;
	char temp;
	while (RAVAIL > 0x1000){
		temp = *(PS2_DATA) & 0xFF;
		RAVAIL = *(PS2_DATA) & 0xFFFF0000;
	}
}

void Input(unsigned int key, unsigned int speed){
	if (mode == GAME){
		if (key == 1){
			pongEngine_paddleMove(1, DOWN, 	speed);
		}
		else if (key == 2){
			pongEngine_paddleMove(1, UP, 	speed);
		}
		else if (key == 3){
			pongEngine_paddleMove(2, DOWN, 	speed);
		}
		else if (key == 4){
			pongEngine_paddleMove(2, UP, 	speed);
		}
	}
}

void InputMode(unsigned int _mode){
	if (_mode == MENUS) { mode = MENUS; } else { mode = GAME; }
}
