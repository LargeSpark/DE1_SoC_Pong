/*
 * pongScreens.c
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

#include "pongScreens.h"
#include <string.h>

volatile unsigned int menuSelector = 0;
volatile unsigned int settings[] = {0,0,4,0,0};
volatile unsigned int menuSelectorOld = 0;
volatile unsigned int settingsOld[] = {0,0,4,0,0};
short menuColours[] = {_BLUE, _BLACK, _BLACK, _BLACK, _BLACK};

unsigned int settingsMax[] = {1, 1, 9, 1, 1};

unsigned int numMenuItems = sizeof(settings)/sizeof(int);
volatile unsigned int gameMode = GAME_AI;

/*
signed int settingStore[4][4] = { { , , , },
								{ , , , },
								{ , , , },
								{ , , , } };

signed int settingStore[4][4] = { { , , , },
								{ , , , },
								{ , , , },
								{ , , , } };
*/

void setMenu(unsigned int _menuSelector, unsigned int _setting){
	menuSelectorOld = menuSelector;
	settingsOld[_menuSelector] = settings[_menuSelector];

	menuSelector = _menuSelector;
	settings[_menuSelector] = _setting;
}

void menuMove(unsigned int direction){
	unsigned int i;

	for (i = 0; i<numMenuItems; i++){
		settingsOld[i] = settings[i];
		menuColours[i] = _BLACK;
	}

	menuSelectorOld = menuSelector;

	if (direction == _DOWN){
		if (menuSelector == 4){ menuSelector = 0; } else menuSelector++;
		//menuSelector++;
	} else if (direction == _UP){
		if (menuSelector == 0){ menuSelector = 4; } else menuSelector--;
		//menuSelector--;
	} else if (direction == _LEFT){
		if (settings[menuSelector] == 0) { settings[menuSelector] = settingsMax[menuSelector]; } else settings[menuSelector]--;
		//settings[menuSelector]--;
	} else if (direction == _RIGHT){
		if (settings[menuSelector] == settingsMax[menuSelector]) { settings[menuSelector] = 0; } else settings[menuSelector]++;
		//settings[menuSelector]++;
	}
	//printf("%d %d\n", menuSelector, settings[menuSelector]);// For debug only

	// Instantaneous effects
	if (menuSelector == 3){
		if (settings[3] == 1){ 			// If press right on Reset
			settings[3] = 0;			// Reset setting
			pongEngine_resetScore(0); 	// Reset scores
		}
	}

	if (menuSelector == 4){	// Start game
		if (settings[4] == 1){ 		// If press right on start
			settings[4] = 0;		// Reset start setting
			setInputMode(gameMode); 	// Begin playing
		}
	}

	menuColours[menuSelector] = _BLUE;
	usleep(200000); //paddleBeep();  // Prevent keyboard bounce
	emptyFIFO();
	ResetWDT();
}

void startScreen(){
	// Clear screen and set input mode
	//Displays_fillColour(_GREEN);
	Displays_fillColour(_RED);
	setInputMode(MENUS);

	pongSprites_writeText(60, 120, 1, "Welcome to armPONG", 0xFFFF);
	Displays_forceRefresh();
	ResetWDT();

	startSound();
	usleep(1000000); ResetWDT();
	usleep(1000000); ResetWDT();
}

void gameMenu(){
	unsigned int i;
	char str_txt[30];

	setInputMode(MENUS);
	gameMode = GAME_AI;
	SDisplay_clearAll();

	// Reset menu
	menuSelector = 0; menuSelectorOld = 0;
	for (i = 0; i<numMenuItems; i++){
		menuColours[i] = _BLACK;
	}
	menuColours[0] = _BLUE;

	sprintf(str_txt, "<%d>", settings[2]);

	//Displays_clearScreen();
	Displays_fillColour(_WHITE);

	//Displays_forceRefresh();
	// Menu items
	pongSprites_renderBall(50, 71/*+25*menuSelector*/, _BLACK); ResetWDT();
	pongSprites_writeText(100, 20, 1, "Main menu", _RED); ResetWDT();
	pongSprites_writeText(75, 65, 1, "Mode: ", menuColours[0]); ResetWDT();
	pongSprites_writeText(75, 90, 1, "Paddle size: ", menuColours[1]); ResetWDT();
	pongSprites_writeText(75, 115, 1, "Volume: ", menuColours[2]); ResetWDT();
	pongSprites_writeText(75, 140, 1, "Reset score > ", menuColours[3]); ResetWDT();
	pongSprites_writeText(75, 165, 1, "START >", menuColours[4]); ResetWDT();

	// Options (Defaults)
	pongSprites_writeText(240, 65, 1, "<AI>", _MAGENTA); ResetWDT();

	pongSprites_writeText(242, 115, 1, str_txt, _MAGENTA); ResetWDT();

	ResetWDT();
	//Displays_forceRefresh(); // Hmm that frame buffer's being odd again
	Displays_Refresh();
	enableInputs(1);
	while(getInputMode() == MENUS){
		if ((menuSelector != menuSelectorOld) || (memcmp(&settings, &settingsOld, sizeof(int)) != 0)){
			// Clear screen and set input mode
			//Displays_clearScreen();

			pongSprites_renderBall(50, (71+(int)25*menuSelectorOld), _WHITE); ResetWDT(); // Get rid of old ball

			//Displays_forceRefresh();

			// Menu items
			pongSprites_renderBall(50, (71+(int)25*menuSelector), _BLACK); ResetWDT();
			pongSprites_writeText(100, 20, 1, "Main menu", _RED); ResetWDT();
			pongSprites_writeText(75, 65, 1, "Mode: ", menuColours[0]); ResetWDT();
			pongSprites_writeText(75, 90, 1, "Paddle size: ", menuColours[1]); ResetWDT();
			pongSprites_writeText(75, 115, 1, "Volume: ", menuColours[2]); ResetWDT();
			pongSprites_writeText(75, 140, 1, "Reset score > ", menuColours[3]); ResetWDT();
			pongSprites_writeText(75, 165, 1, "START >", menuColours[4]); ResetWDT();

			// Options -- Requires work
			if (menuSelector == 0){ // Game mode
				if (settings[0] == 1){ 			// If press right on Reset
					pongSprites_writeText(240, 65, 1, "<AI>", _WHITE); ResetWDT();
					pongSprites_writeText(240, 65, 1, "<2P>", _MAGENTA); ResetWDT();
					gameMode = GAME;
				} else {
					pongSprites_writeText(240, 65, 1, "<2P>", _WHITE); ResetWDT();
					pongSprites_writeText(240, 65, 1, "<AI>", _MAGENTA); ResetWDT();
					gameMode = GAME_AI;
				}
			}

			if (menuSelector == 2){ // Volume -- requires work
				pongSprites_writeText(242, 115, 1, str_txt, _WHITE); ResetWDT();
				sprintf(str_txt, "<%d>", settings[2]);
				pongSprites_writeText(242, 115, 1, str_txt, _MAGENTA); ResetWDT();
				setVolume(settings[2]);
			}

			Displays_forceRefresh();

			menuSelectorOld = menuSelector;
			for (i = 0; i<numMenuItems; i++){
				settingsOld[i] = settings[i];
				menuColours[i] = _BLACK;
			}
		}
	ResetWDT();
	}
	Displays_clearScreen();
}

void testScreen_AI( void ){
	int dir = 270;
	int x;
	int variable = 240; //240

	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(GAME_AI);
	pongEngine_resetBallLoc();

	// Initialise engine
	pongEngine_init();
	ResetWDT();

	pongEngine_createBall();
	pongSprites_writeText(96, 60, 1, "AI MODE", 0xFFFF);
	pongSprites_writeText(96, 90, 0, "TESTICLES", 0xFFFF);
	Displays_forceRefresh(); pongEngine_refreshScore();
	while (getInputMode() == GAME_AI){
		ResetWDT();
		Displays_forceRefresh();

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
	Displays_clearScreen();
}

void testScreen( void ){
	int dir = 0;
	int x;
	int variable = 240; //240

	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(GAME);

	pongEngine_resetBallLoc();

	// Initialise engine
	pongEngine_init();
	ResetWDT();

	pongEngine_createBall();
	pongSprites_writeText(96, 60, 1, "2P MODE", 0xFFFF);
	pongSprites_writeText(96, 90, 0, "TESTICLES", 0xFFFF);
	Displays_forceRefresh(); pongEngine_refreshScore();
	while (getInputMode() == GAME){
		ResetWDT();
		Displays_forceRefresh();

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
	Displays_clearScreen();
}
