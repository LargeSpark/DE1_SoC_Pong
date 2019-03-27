#include "DE1Soc_VGA/DE1SoC_VGA.h"
#include "pongDisplay/pongDisplay.h"
#include "pongEngine/pongSprites.h"
int main(void){
//VGA TESTCODE
	int x;
	Displays_init(0xC8000000,0xC9000000,0xFF200060,0xFF200080);
	Displays_mode(HARDWAREFB);
	pongSprites_init();
	Displays_clearScreen();

	while(1){
	for (x=0;x<320-17;x++)
	{
		pongSprites_renderBall(x, 70,0xffff);
		Displays_Refresh();
		pongSprites_renderBall(x, 70,0x0000);
		usleep(1000);
	}
	for (x=320-17;x>0;x--)
	{
		pongSprites_renderBall(x, 70,0xffff);
		Displays_Refresh();
		pongSprites_renderBall(x, 70,0x0000);
		usleep(1000);
	}
	}
}
