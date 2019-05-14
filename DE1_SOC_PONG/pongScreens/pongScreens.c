/*
 * pongScreens.c
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

#include "pongScreens.h"
//#include <string.h>

volatile unsigned int menuSelector = 0;
volatile unsigned int settings[] = {0,5,4,0,0};
volatile unsigned int menuSelectorOld = 0;
volatile unsigned int settingsOld[] = {0,5,4,0,0};
short menuColours[] = {_BLUE, _BLACK, _BLACK, _BLACK, _BLACK};

unsigned int settingsMax[] = {1, 9, 9, 1, 1};

unsigned int numMenuItems = sizeof(settings)/sizeof(int);
volatile unsigned int gameMode 		= GAME_AI;
volatile unsigned int gameModeOld 	= GAME_AI;
int n = 0;
volatile unsigned int *slider_ptr = (unsigned int *)0xFF200040;
unsigned int last_slider;

void setMenu(unsigned int _menuSelector, unsigned int _setting){
	menuSelectorOld = menuSelector;
	settingsOld[_menuSelector] = settings[_menuSelector];

	menuSelector = _menuSelector;
	settings[_menuSelector] = _setting;
}

void menuMove(unsigned int direction){
	unsigned int i;
	char str_txt[30];
	enableInputs(0);

	// Save old settings and reset menu colours
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


	/* Set menu item colours */
	if (menuSelector != menuSelectorOld){
		menuColours[menuSelector] = _BLUE;
		pongSprites_renderBall(50, (71+(int)25*menuSelectorOld), _WHITE); ResetWDT(); // Get rid of old ball
		pongSprites_renderBall(50, (71+(int)25*menuSelector), _BLACK); ResetWDT();   // Replace
		// Menu items
		pongSprites_writeText(100, 20, 	LARGE, "Main menu", _RED); ResetWDT();
		pongSprites_writeText(75, 65, 	LARGE, "Mode: ", menuColours[0]); ResetWDT();
		pongSprites_writeText(75, 90, 	LARGE, "Paddle size: ", menuColours[1]); ResetWDT();
		pongSprites_writeText(75, 115, 	LARGE, "Volume: ", menuColours[2]); ResetWDT();
		pongSprites_writeText(75, 140, 	LARGE, "Reset score > ", menuColours[3]); ResetWDT();
		pongSprites_writeText(75, 165, 	LARGE, "START >", menuColours[4]); ResetWDT();
	}

	/* Menu effects */
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

	if ((menuSelector == 1) && (settings[menuSelector] != settingsOld[menuSelector])){ // Paddle size
		sprintf(str_txt, "<%d>", settingsOld[1]);
		pongSprites_writeText(242, 90, LARGE, str_txt, _WHITE); ResetWDT();
		sprintf(str_txt, "<%d>", settings[1]);
		pongSprites_writeText(242, 90, LARGE, str_txt, _MAGENTA); ResetWDT();

		pongSprites_changePaddleSize(settings[1]);
	}

	if ((menuSelector == 2) && (settings[menuSelector] != settingsOld[menuSelector])){ // Volume
		sprintf(str_txt, "<%d>", settingsOld[2]);
		pongSprites_writeText(242, 115, LARGE, str_txt, _WHITE); ResetWDT();
		sprintf(str_txt, "<%d>", settings[2]);
		pongSprites_writeText(242, 115, LARGE, str_txt, _MAGENTA); ResetWDT();

		if (settings[2] != 0){
			enableSound(1);
			setVolume(settings[2]);
		} else {
			enableSound(0); // Disable sound at volume 0
		}

		//Sound(G4, 50); ResetWDT();
		//paddleBeep();
	}

	if (menuSelector == 3){
		if (settings[3] == 1){ 			// If press right on Reset
			settings[3] = 0;			// Reset setting
			pongEngine_resetScore(0); 	// Reset scores
		}
	}

	if (menuSelector == 4){	// Start game
		if (settings[4] == 1){ 		// If press right on start
			settings[4] = 0;		// Reset start setting
			if (gameModeOld != gameMode){ // Reset score for new game modes
				pongEngine_resetScore(0);
			}
			// Ensure last ball is gone for animation
			pongSprites_renderRectangle(50-8, 50+8, 71-8, 180, _WHITE);
			for (i = 50; i < 260; i++){
				pongSprites_renderBall(i, 	(71+(int)25*menuSelector), _WHITE); ResetWDT(); // Get rid of old ball
				pongSprites_renderBall(i+1, (71+(int)25*menuSelector), _BLACK); ResetWDT();   // Replace
			}
			setInputMode(gameMode); 	// Begin playing
		}
	}

	usleep(150000); //paddleBeep();  // Prevent keyboard input bounce
	emptyFIFO();
	ResetWDT();
	enableInputs(1);
}

void startScreen(){
	// Clear screen and set input mode
	Displays_fillColour(_RED); 	// Fill background
	setInputMode(MENUS);		// Ensure input mode set to menus

	// Write greeting and refresh
	pongSprites_writeText(60, 120, 1, "Welcome to armPONG", 0xFFFF);
	Displays_forceRefresh();
	ResetWDT();

	startSound();					// Play startup sound
	usleep(1000000); ResetWDT();	// Sleep for 2s
	usleep(1000000); ResetWDT();
	last_slider = *slider_ptr; // Ensure slider positions are saved
}

void gameMenu(){
	unsigned int i;
	char str_txt[30];

	setInputMode(MENUS);
	gameModeOld = gameMode;
	SDisplay_clearAll();

	// Reset menu
	menuSelector = 0; menuSelectorOld = 0;
	for (i = 0; i<numMenuItems; i++){
		menuColours[i] = _BLACK;
	}
	menuColours[0] = _BLUE;

	//Displays_clearScreen();
	Displays_fillColour(_WHITE);

	//Displays_forceRefresh();
	// Menu items
	pongSprites_renderBall(50, 71/*+25*menuSelector*/, _BLACK); ResetWDT();
	pongSprites_writeText(100, 20, 	LARGE, "Main menu", _RED); ResetWDT();
	pongSprites_writeText(75, 65, 	LARGE, "Mode: ", menuColours[0]); ResetWDT();
	pongSprites_writeText(75, 90, 	LARGE, "Paddle size: ", menuColours[1]); ResetWDT();
	pongSprites_writeText(75, 115, 	LARGE, "Volume: ", menuColours[2]); ResetWDT();
	pongSprites_writeText(75, 140, 	LARGE, "Reset score > ", menuColours[3]); ResetWDT();
	pongSprites_writeText(75, 165, 	LARGE, "START >", menuColours[4]); ResetWDT();

	// Options (Defaults)
	if (settings[0] == 0){
		pongSprites_writeText(240, 65, LARGE, "<AI>", _MAGENTA); ResetWDT();
	} else {
		pongSprites_writeText(240, 65, LARGE, "<2P>", _MAGENTA); ResetWDT();
	}

	sprintf(str_txt, "<%d>", settings[1]); // Paddlesize options
	pongSprites_writeText(242, 90, LARGE, str_txt, _MAGENTA); ResetWDT();

	sprintf(str_txt, "<%d>", settings[2]); // Volume options
	pongSprites_writeText(242, 115, LARGE, str_txt, _MAGENTA); ResetWDT();

	ResetWDT();
	//Displays_forceRefresh(); // Hmm that frame buffer's being odd again
	Displays_Refresh();
	enableInputs(1);
	while(getInputMode() == MENUS){
		/* Most of this can probably be pushed to menuMove */

		if ((menuSelector != menuSelectorOld) || (settings[menuSelector] != settingsOld[menuSelector])){
			// As sounds are blocking, and rely on a timer ISR, they need to be called outside menuMove,
			// as it is essentially an extension of the input ISRs
			if (settings[2] != settingsOld[2]){
				Sound(G4, 50);
			}

			//Displays_forceRefresh();

			menuSelectorOld = menuSelector;
			for (i = 0; i<numMenuItems; i++){
				settingsOld[i] = settings[i];
				menuColours[i] = _BLACK;
			}
		}
	ResetWDT();
	}
	last_slider = *slider_ptr;
	Displays_clearScreen();
}

void testScreen_AI( void ){
	int dir;
	int vel;
	int *arr;
	int x;
	int variable = 240;

	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(GAME_AI);

	// Initialise engine
	pongEngine_init();
	ResetWDT();

	pongEngine_createBall();
	//pongSprites_writeText(96, 60, 1, "AI MODE", 0xFFFF);

	Displays_forceRefresh(); pongEngine_refreshScore();
	n = 0;
	while (getInputMode() == GAME_AI){

		ResetWDT();
		Displays_forceRefresh();

		if (n < 1) {

					n++;
					dir = pongPhysics_serve ();
					vel = 2;
			}

		if ((pongEngine_getBallLocation_x() <= 50 + 15) && (pongEngine_getBallLocation_x() >= 50 + 10))   {

			if ((pongEngine_getBallLocation_y() <= pongEngine_getPaddleY (1) + 30 + 15) && (pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (1) - 30 - 15)) {

			paddleBeep();
		    arr = pongPhysics_paddleCollision(vel,dir,1);
		    dir = arr[0];
		    vel = arr[1];

			}

		} else if ((pongEngine_getBallLocation_x() >= 270 - 15) && (pongEngine_getBallLocation_x() <= 270 - 10))  { // -10

			if ((pongEngine_getBallLocation_y() <= pongEngine_getPaddleY (2) + 30 + 15) && (pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (2) - 30 - 15))  {

			paddleBeep();
			arr = pongPhysics_paddleCollision(vel,dir,2);
			dir = arr[0];
			vel = arr[1];


			}
		}

		//collisions
		if ( pongEngine_getBallLocation_y() <= 23 + 20  ) {
			enableInputs(0);
			paddleBeep();
			enableInputs(1);

			dir = pongPhysics_borderCollision(vel, dir);


		} else if ( pongEngine_getBallLocation_y() >= 230 - 10){
			enableInputs(0);
			ballOutBeep();
			enableInputs(1);

			dir = pongPhysics_borderCollision(vel, dir);
		}



		// AI bit
		if ( pongEngine_getBallLocation_y() > pongEngine_getPaddleY(2)){

			pongEngine_paddleMove(2, UP, 2);

		} else if ( pongEngine_getBallLocation_y() < pongEngine_getPaddleY(2)){

			pongEngine_paddleMove(2, DOWN, 	2);
		}

		pongEngine_moveBall(dir, vel);

		if (*slider_ptr != last_slider)	{

			setInputMode(MENUS);
			last_slider = *slider_ptr;
		}
		//This function adds points
		if ( pongEngine_getBallLocation_x() <= 10  ) { //+20
				pongEngine_destroyBall(); //destroy ball
				Displays_forceRefresh(); //force refresh for buffers prior to points
				enableInputs(0); //disable inputs
				paddleBeep(); //play sound
				enableInputs(1); //enable inputs
				pongEngine_resetPaddles(); //reset paddles
				pongEngine_resetBallLoc(); //reset ball location
				pongEngine_createBall(); //create ball
				pongEngine_paddleCreate(1); //create paddle
				pongEngine_paddleCreate(2); //create paddle
				pongEngine_moveBall(1, 0); //reset angle instructions
				pongEngine_moveBall(0, 0);
				Displays_forceRefresh(); pongEngine_refreshScore(); //force refresh
				variable = 240; //reset var
				n = 0; //reset var
				pongEngine_addPoint(2); //add point
		} else if ( pongEngine_getBallLocation_x() >= 320 - 10){
			pongEngine_destroyBall(); //destroy ball
			enableInputs(0); //disable inputs
			paddleBeep(); //play sound
			enableInputs(1); //enable inputs
			pongEngine_resetPaddles(); //reset paddles
			pongEngine_resetBallLoc(); //reset ball location
			pongEngine_createBall(); //create ball
			pongEngine_paddleCreate(1); //create paddle
			pongEngine_paddleCreate(2); //create paddle
			pongEngine_moveBall(1, 0); //reset angle instructions
			pongEngine_moveBall(0, 0);
			Displays_forceRefresh(); pongEngine_refreshScore(); //force refresh
			variable = 240; //reset var
			n = 0; //reset var
			pongEngine_addPoint(1); //add point


		}
	}

	//gameEngine_paddleDestroy(1);
	//gameEngine_paddleDestroy(2);
	Displays_clearScreen();
}

void testScreen( void ){
	int dir = 0;
	int vel;
	int* arr;
	int x;
	int variable = 240;
	// Clear screen and set input mode
	Displays_clearScreen();
	setInputMode(GAME);

	// Initialise engine
	pongEngine_init();
	ResetWDT();

	pongEngine_createBall();
	//pongSprites_writeText(96, 60, 1, "2P MODE", 0xFFFF);
	Displays_forceRefresh(); pongEngine_refreshScore();
	n = 0;
	while (getInputMode() == GAME){
		ResetWDT();
		Displays_forceRefresh();

		if (n < 1) {
					n++;
					dir = pongPhysics_serve ();
					vel = 3;
			}

		if ((pongEngine_getBallLocation_x() <= 50 + 15) && (pongEngine_getBallLocation_x() >= 50 + 10))   {

			if ((pongEngine_getBallLocation_y() <= pongEngine_getPaddleY (1) + 30) && (pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (1) - 30)) {

			paddleBeep();
		    arr = pongPhysics_paddleCollision(vel,dir,1);
		    dir = arr[0];
		    vel = arr[1];

			}
		} else if ((pongEngine_getBallLocation_x() >= 270 - 15) && (pongEngine_getBallLocation_x() <= 270 - 10))  { // -10

			if ((pongEngine_getBallLocation_y() <= pongEngine_getPaddleY (2) + 30) && (pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (2) - 30))  {

			paddleBeep();
			arr = pongPhysics_paddleCollision(vel,dir,2);
			dir = arr[0];
			vel = arr[1];


			}
		}

		//collisions
		if ( pongEngine_getBallLocation_y() <= 23 + 20  ) {
			enableInputs(0);
			paddleBeep();
			enableInputs(1);

			dir = pongPhysics_borderCollision(vel, dir);


		} else if ( pongEngine_getBallLocation_y() >= 230 - 10){
			enableInputs(0);
			ballOutBeep();
			enableInputs(1);

			dir = pongPhysics_borderCollision(vel, dir);
		}

		//This function adds points
		if ( pongEngine_getBallLocation_x() <= 10  ) { //+20
			pongEngine_destroyBall(); //destroy ball
			Displays_forceRefresh(); //force refresh for buffers prior to points
			enableInputs(0); //disable inputs
			paddleBeep(); //play sound
			enableInputs(1); //enable inputs
			pongEngine_resetPaddles(); //reset paddles
			pongEngine_resetBallLoc(); //reset ball location
			pongEngine_createBall(); //create ball
			pongEngine_paddleCreate(1); //create paddle
			pongEngine_paddleCreate(2); //create paddle
			pongEngine_moveBall(1, 0); //reset angle instructions
			pongEngine_moveBall(0, 0);
			Displays_forceRefresh(); pongEngine_refreshScore(); //force refresh
			variable = 240; //reset var
			n = 0; //reset var
			pongEngine_addPoint(2); //add point
		} else if ( pongEngine_getBallLocation_x() >= 320 - 10){
			pongEngine_destroyBall(); //destroy ball
			enableInputs(0); //disable inputs
			paddleBeep(); //play sound
			enableInputs(1); //enable inputs
			pongEngine_resetPaddles(); //reset paddles
			pongEngine_resetBallLoc(); //reset ball location
			pongEngine_createBall(); //create ball
			pongEngine_paddleCreate(1); //create paddle
			pongEngine_paddleCreate(2); //create paddle
			pongEngine_moveBall(1, 0); //reset angle instructions
			pongEngine_moveBall(0, 0);
			Displays_forceRefresh(); pongEngine_refreshScore(); //force refresh
			variable = 240; //reset var
			n = 0; //reset var
			pongEngine_addPoint(1); //add point
		}

		pongEngine_moveBall(dir, vel);

		if (*slider_ptr != last_slider){
			setInputMode(MENUS);
			last_slider = *slider_ptr;
		}
	}
	//gameEngine_paddleDestroy(1);
	//gameEngine_paddleDestroy(2);
	Displays_clearScreen();
}
