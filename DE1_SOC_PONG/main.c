#include "DE1Soc_VGA/DE1SoC_VGA.h"
#include "pongDisplay/pongDisplay.h"
#include "pongEngine/pongSprites.h"
#include "pongEngine/pongEngine.h"
#include "HPS_Watchdog/HPS_Watchdog.h"

void testScreen();

int main(void){
//VGA TESTCODE

	int FS = 3;
	int paddleX;
	Displays_init(0xC8000000,0xC9000000,0xFF200060,0xFF200080);
	Displays_mode(SOFTWAREOCTOFB);
	Displays_frameSkip(FS);
	pongEngine_init();
	pongEngine_refreshScore();
	testScreen();
}

void testScreen(){
	int x;
	int variable = 240; //240
	pongEngine_createBall();
	pongSprites_writeText(96, 60, 1, "TEST SCREEN", 0xFFFF);
	pongSprites_writeText(96, 90, 0, "TEST SCREEN", 0xFFFF);
		while(1){
			//pongEngine_moveBall(0,1);

			ResetWDT();
			Displays_Refresh();

			for(x=0;x<variable;x++){
				pongEngine_paddleMove(1, UP, 1);
				pongEngine_paddleMove(2, DOWN, 2);
				pongEngine_moveBall(180, 1);
				Displays_Refresh();
			}
			pongEngine_addPoint(1);
			for(x=variable;x>=0;x--){
					pongEngine_paddleMove(1, DOWN, 1);
					pongEngine_paddleMove(2, UP, 2);
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
