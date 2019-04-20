/*
 * pongScreens.c
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

#include "pongScreens.h"

void startScreen(){
	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(MENUS);


	pongSprites_writeText(60, 120, 1, "Welcome to armPONG", _RED); ResetWDT();
	usleep(1000000); ResetWDT();
	usleep(1000000); ResetWDT();
	usleep(1000000); ResetWDT();
}

void gameMenu(){
	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(MENUS);

	enableInputs(1);

	pongSprites_writeText(60, 120, 1, "I am a menu!", _RED); ResetWDT();
	pongSprites_writeText(60, 180, 1, "Press [Enter] or [PB3]", _GREEN); ResetWDT();
	while(getInputMode() == MENUS){
		ResetWDT();
	}
}

void testScreen( void ){
	int x;
	int variable = 240; //240

	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(GAME);

	// Initialise engine
	pongEngine_init();
	pongEngine_refreshScore();
	ResetWDT();

	pongEngine_createBall();
	pongSprites_writeText(96, 60, 1, "TEST SCREEN", 0xFFFF);
	pongSprites_writeText(96, 90, 2, "TEST SCREEN", 0xFFFF);
	while(getInputMode() == GAME){
		ResetWDT();
		Displays_Refresh();

		for(x=0;x<2*variable;x++){
			pongEngine_moveBall(180, 1);
			Displays_Refresh();
		}
		pongEngine_addPoint(1);
		enableInputs(0);
		Sound(1000,100);
		enableInputs(1);
		for(x=variable;x>=0;x--){
			pongEngine_moveBall(0, 2);
			Displays_Refresh();
		}
		pongEngine_addPoint(2);
		enableInputs(0);
		Sound(250,100);
		enableInputs(1);
	}
}


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
