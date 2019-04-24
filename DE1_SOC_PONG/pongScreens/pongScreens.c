/*
 * pongScreens.c
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

#include "pongScreens.h"

unsigned int menuSelector = 0;
unsigned int settings[] = {0,0,0};
unsigned int menuSelectorOld = 0;
unsigned int settingsOld[] = {0,0,0};

void menuMove(unsigned int direction){
	if (direction == _DOWN){
		menuSelector++;
	} else if (direction == _UP){
		menuSelector--;
	} else if (direction == _LEFT){
		settings[menuSelector]--;
	} else if (direction == _RIGHT){
		settings[menuSelector]++;
	}
}

void startScreen(){
	// Clear screen and set input mode
	Displays_fillColour(_GREEN);
	setInputMode(MENUS);


	pongSprites_writeText(60, 120, 1, "Welcome to armPONG", _RED); ResetWDT();
	Displays_Refresh();
	startSound();
	usleep(1000000); ResetWDT();
	usleep(1000000); ResetWDT();
}

void gameMenu(){
	setInputMode(MENUS);

	Displays_fillColour(_WHITE);

	enableInputs(1);

	pongSprites_writeText(90, 20, 1, "Main menu", _RED); ResetWDT();

	// Menu items
	pongSprites_renderBall(50, 73, _BLACK); ResetWDT();
	pongSprites_writeText(75, 65, 1, "Mode: ", _BLACK); ResetWDT();
	pongSprites_writeText(75, 90, 1, "Difficulty: ", _BLACK); ResetWDT();
	pongSprites_writeText(75, 115, 1, "Volume: ", _BLACK); ResetWDT();
	pongSprites_writeText(75, 140, 1, "Start!", _BLACK); ResetWDT();

	pongSprites_writeText(30, 180, 1, "Press [Enter] or [PB3]", _MAGENTA); ResetWDT();
	ResetWDT();
	Displays_Refresh();
	while(getInputMode() == MENUS){

		if ((menuSelector != menuSelectorOld)){//(settings != settingsOld) ||
			// Clear screen and set input mode
			Displays_fillColour(_WHITE);

			enableInputs(1);

			pongSprites_writeText(90, 20, 1, "Main menu", _RED); ResetWDT();

			// Menu items
			pongSprites_renderBall(50, 73, _BLACK); ResetWDT();
			pongSprites_writeText(75, 65, 1, "Mode: ", _BLACK); ResetWDT();
			pongSprites_writeText(75, 90, 1, "Difficulty: ", _BLACK); ResetWDT();
			pongSprites_writeText(75, 115, 1, "Volume: ", _BLACK); ResetWDT();
			pongSprites_writeText(75, 140, 1, "Start!", _BLACK); ResetWDT();

			pongSprites_writeText(30, 180, 1, "Press [Enter] or [PB3]", _MAGENTA); ResetWDT();
			Displays_Refresh();
		}
	ResetWDT();
	}
}

void testScreen_AI( void ){
	int dir = 270;
	int x;
	int variable = 240; //240

	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(GAME_AI);

	// Initialise engine
	pongEngine_init();
	ResetWDT();

	pongEngine_createBall();
	pongSprites_writeText(96, 60, 1, "TEST SCREEN", 0xFFFF);
	pongSprites_writeText(96, 90, 0, "TESTICLES", 0xFFFF);
	Displays_Refresh(); pongEngine_refreshScore();
	while (getInputMode() == GAME_AI){
		ResetWDT();
		Displays_Refresh();

		if ( pongEngine_getBallLocation_y() <= 25 ) {
			pongEngine_addPoint(2);
			enableInputs(0);
			paddleBeep();
			enableInputs(1);
			dir = 270;
			pongEngine_moveBall(dir, 2);
		} else if ( pongEngine_getBallLocation_y() >= 230 ){
			pongEngine_addPoint(1);
			enableInputs(0);
			ballOutBeep();
			enableInputs(1);
			dir = 90;
			pongEngine_moveBall(dir, 2);
		}

		// AI bit
		if ( pongEngine_getBallLocation_y() > pongEngine_getPaddleY(2)){
			pongEngine_paddleMove(2, UP, 	1);
		} else if ( pongEngine_getBallLocation_y() < pongEngine_getPaddleY(2)){
			pongEngine_paddleMove(2, DOWN, 	1);
		}

		pongEngine_moveBall(dir, 2);
	}
	//gameEngine_paddleDestroy(1);
	//gameEngine_paddleDestroy(2);
}

void testScreen( void ){
	int dir = 0;
	int x;
	int variable = 240; //240

	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(GAME);

	// Initialise engine
	pongEngine_init();
	ResetWDT();

	pongEngine_createBall();
	pongSprites_writeText(96, 60, 1, "TEST SCREEN", 0xFFFF);
	pongSprites_writeText(96, 90, 0, "TESTICLES", 0xFFFF);
	Displays_Refresh(); pongEngine_refreshScore();
	while (getInputMode() == GAME){
		ResetWDT();
		Displays_Refresh();

		if ( pongEngine_getBallLocation_x() <= 8 ) {
			pongEngine_addPoint(2);
			enableInputs(0);
			paddleBeep();
			enableInputs(1);
			dir = 180;
			pongEngine_moveBall(dir, 1);
		} else if ( pongEngine_getBallLocation_x() >= 312 ){
			pongEngine_addPoint(1);
			enableInputs(0);
			ballOutBeep();
			enableInputs(1);
			dir = 0;
			pongEngine_moveBall(dir, 1);
		}
		pongEngine_moveBall(dir, 1);
	}
	//gameEngine_paddleDestroy(1);
	//gameEngine_paddleDestroy(2);
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
