#include "DE1Soc_VGA/DE1SoC_VGA.h"
#include "pongDisplay/pongDisplay.h"
#include "pongEngine/pongSprites.h"
#include "pongEngine/pongEngine.h"
#include "pongInputs/pongInputs.h"
#include "HPS_Watchdog/HPS_Watchdog.h"


void testScreen( void ){
	int x;
	int variable = 240; //240
	pongEngine_createBall();
	pongSprites_writeText(96, 60, 1, "TEST SCREEN", 0xFFFF);
	pongSprites_writeText(96, 90, 0, "TEST SCREEN", 0xFFFF);
	while(1){
		//pongEngine_moveBall(0,1);

		ResetWDT();
		Displays_Refresh();

		for(x=0;x<2*variable;x++){
			/*if (paddleDir(1) != -1){
				pongEngine_paddleMove(1, paddleDir(1), 1);
			}
			if (paddleDir(2) != -1){
				pongEngine_paddleMove(2, paddleDir(2), 1);
			}*/
			pongEngine_moveBall(180, 1);
			Displays_Refresh();
		}
		pongEngine_addPoint(1);
		for(x=variable;x>=0;x--){
			/*if (paddleDir(1) != -1){
				pongEngine_paddleMove(1, paddleDir(1), 2);
			}
			if (paddleDir(2) != -1){
				pongEngine_paddleMove(2, paddleDir(2), 2);
			}*/
			pongEngine_moveBall(0, 2);
			Displays_Refresh();
		}
		pongEngine_addPoint(2);
		//pongEngine_destroyBall();
	/*for (x=8;x<320-9;x++)
	{
		pongSprites_renderBall(x, 70,LT24_BLUE);
		if(x<180){ paddleX = x; }
		pongSprites_renderPaddle(50, 180-paddleX, 0xFFFF);
		pongSprites_renderPaddle(270, paddleX, 0xFFFF);
		Displays_Refresh();
		pongSprites_renderBall(x, 70,0x0000);
		pongSprites_renderPaddle(50, 180-x, 0x0000);
		pongSprites_renderPaddle(270, x, 0x0000);
	}
	for (x=320-9;x>8;x--)
	{
		pongSprites_renderBall(x, 70,LT24_RED);
		if(x<180){ paddleX = x; }
		pongSprites_renderPaddle(50, 180-paddleX, 0xFFFF);
		pongSprites_renderPaddle(270, paddleX, 0xFFFF);
		Displays_Refresh();
		pongSprites_renderBall(x, 70,0x0000);
		pongSprites_renderPaddle(50, 180-x, 0x0000);
		pongSprites_renderPaddle(270, x, 0x0000);
	}*/
	/*
	FS++;
	Displays_frameSkip(FS);
	*/
	}
}

int main(void) {
	int FS = 3;
	int paddleX;
	Displays_init(0xC8000000,0xC9000000,0xFF200060,0xFF200080);
	Displays_mode(0);
	Displays_frameSkip(FS);
	ResetWDT();
    //Initialise keyboard

	pongEngine_init();
	pongEngine_refreshScore();
	ResetWDT();

	inputsInitialise();
	testScreen();
}



//    /*
//     *  Declare pointers to I/O registers (volatile keyword means memory not cached)
//     */
//
//    // WDT Current Value  Register (l4wd0: wdt_ccvr)
//    volatile unsigned int *wdt_ccvr = (unsigned int *) 0xFFD02008;
//    // WDT Counter Reload Register (l4wd0: wdt_crr)
//    volatile unsigned int *wdt_crr  = (unsigned int *) 0xFFD0200C;
//    // Red LEDs base address
//    volatile int *LEDR_ptr = (int *) 0xFF200000;
//    // KEY buttons base address
//    volatile int *KEY_ptr  = (int *) 0xFF200050;
//
//    /*
//     *  Primary Run Loop.
//     *  Bare-Metal Applications use an infinite loop to keep executing (no OS).
//     */
//     while(1) {
//        // Read WDT counter value, and display it on the red LEDs (scaled to fit)
//        *LEDR_ptr = *wdt_ccvr >> 16;
//        // Check if any of the buttons have been pressed.
//        // Each of the lower 4 bits represents one of the keys.
//        // A key is pressed if the corresponding bit is 1.
//        if (*KEY_ptr & 0xF) {
//            // If any key pressed
//        	if (*KEY_ptr & 0x0) { //? Key up??
//        		*wdt_crr = 0x76; // Reset the Watchdog Timer
//        	}
//        }
//    }


