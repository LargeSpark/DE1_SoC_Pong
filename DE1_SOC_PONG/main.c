#include "DE1Soc_VGA/DE1SoC_VGA.h"
#include "pongDisplay/pongDisplay.h"
#include "pongEngine/pongSprites.h"
int main(void){
//VGA TESTCODE
	int x;
	int FS = 3;
	int paddleX;
	Displays_init(0xC8000000,0xC9000000,0xFF200060,0xFF200080);
	Displays_mode(SOFTWAREOCTOFB);
	Displays_frameSkip(FS);
	pongSprites_init();
	Displays_clearScreen();

	while(1){
		//pongEngine_moveBall(0,1);
		pongEngine_paddleCreate(1);
		pongEngine_paddleCreate(2);
		ResetWDT();
		Displays_Refresh();
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
