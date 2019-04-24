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

volatile unsigned int mode;

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

void pushbuttonISR(HPSIRQSource interruptID, bool isInit, void* initParams) {
    if (!isInit) {
        unsigned int press;
        //Read the Push-button interrupt register
        press = KEY_ptr[3];
        //Then clear the interrupt flag by writing the value back
        KEY_ptr[3] = press;

        if (press == 1){
        	Input(_UP, keySpeed);
        }
        else if (press == 2){
        	Input(_DOWN, keySpeed);
        }
        else if (press == 4){
        	Input(_W, keySpeed);
        }
        else if (press == 8){
        	Input(_S, keySpeed);
        }
    }
    //Reset watchdog.
    HPS_ResetWatchdog();
}


/* Actual functions */

void inputsInitialise(void) {
	if (!inputsIsInit){
		// Register IRQs
		HPS_IRQ_registerHandler(IRQ_LSC_PS2_PRIMARY, keyboardISR);
		HPS_ResetWatchdog();
		HPS_IRQ_registerHandler(IRQ_LSC_KEYS, pushbuttonISR);
		HPS_ResetWatchdog();

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
	//printf("%X \n", inputKey); //Debug for checking scancodes
	return inputKey;
}

void inputKeyboard( void ) {
	char key = PS2Scan();
	keyBuffer[0] = keyBuffer[1];
	keyBuffer[1] = keyBuffer[2];
	keyBuffer[2] = PS2Scan();

	if (key == _W) 				{
		keyBuffer[2] = 1; Input(_W, keyBSpeed);
	} else if (key == _S)		{
		keyBuffer[2] = 2; Input(_S, keyBSpeed);
	} else if (key == _UP) 		{
		keyBuffer[2] = 3; Input(_UP, keyBSpeed);
	} else if (key == _DOWN) 	{
		keyBuffer[2] = 4; Input(_DOWN, keyBSpeed);
	} else if (key == _ESC)		{
		keyBuffer[2] = 5; Input(_ESC, keyBSpeed);
	} else if (key == _RETURN)	{
		keyBuffer[2] = 6; Input(_RETURN, keyBSpeed);
	} else if ((key == _BKSPACE) && (keyBuffer[0] != 7)) {
		keyBuffer[2] = 7; toggleSound();//Input(7, keyBSpeed);
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
		//temp = *(PS2_DATA) & 0xFF;
		temp = *(PS2_DATA) & 0xFF; // Read necessary to clear FIFO
		RAVAIL = *(PS2_DATA) & 0xFFFF0000;
	}
}

void Input(unsigned int key, unsigned int speed){
	if ((mode == GAME) || (mode == GAME_AI)){ // Common to games
		if (key == _W){
			pongEngine_paddleMove(1, DOWN, 	speed);
			pongEngine_destroyBall(); // Partially repairs artifacts - constrains them to path of ball
		}
		else if (key == _S){
			pongEngine_paddleMove(1, UP, 	speed);
			pongEngine_destroyBall();
		} else if (key == _ESC){ // Escape
			setInputMode(MENUS);
		}
	}

	if (mode == GAME){ // 2P only
		if (key == _UP){
			pongEngine_paddleMove(2, DOWN, 	speed);
			pongEngine_destroyBall();
		}
		else if (key == _DOWN){
			pongEngine_paddleMove(2, UP, 	speed);
			pongEngine_destroyBall();
		}
	}


	if (mode == MENUS){
		if ((key == _RETURN) || ((key == _S) && (speed == keySpeed))){
			setInputMode(GAME_AI);
		}
	}


	if (key == _BKSPACE){
		toggleSound();
	}
}

void enableInputs(int enable){
	if (enable == 1){
		// Enable interrupts
		KEY_ptr[2] = 0xF; 		// Set key interrupts
		//*PS2_CONTROL |= 0x1; 	// Set interrupt on PS2
		*PS2_CONTROL = (1<<0);
	} else {
		// Disable interrupts
		KEY_ptr[2] = 0x00; 		// Set key interrupts
		//*PS2_CONTROL &= 0x0; 	// Set interrupt on PS2
		*PS2_CONTROL = (0<<0);
	}
}

void setInputMode(unsigned int _mode){
	//if (_mode == MENUS) { mode = MENUS; } else if (_mode == GAME){ mode = GAME; } else if (_mode == GAME_AI){ mode = GAME; }
	mode = _mode;
}

int getInputMode( void ){
	return mode;
}
