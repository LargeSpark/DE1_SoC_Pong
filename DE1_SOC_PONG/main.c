#include "DE1Soc_VGA/DE1SoC_VGA.h"

int main(void){
//VGA TESTCODE
	int x;
	Displays_init(0xC8000000,0x09000000,0xFF200060,0xFF200080);
	Displays_clearScreen();

	while(1){
	for (x=0;x<320;x++)
	{
	  // Draw a straight line in red across the screen centre
	   Displays_drawPixel(x, 59, 0xf800);
	   //Displays_drawPixel(x, 59, 0x07e0);
	  // Draw a "diagonal" line in green
	  if (x<240)
		  //Displays_drawPixel(x, x, 0xf800);
		  Displays_drawPixel(x, x, 0x07e0);
	}
	usleep(1000000);
	for (x=0;x<320;x++)
		{
		  // Draw a straight line in red across the screen centre
		   //Displays_drawPixel(x, 59, 0xf800);
		   Displays_drawPixel(x, 59, 0x07e0);
		  // Draw a "diagonal" line in green
		  if (x<240)
			  Displays_drawPixel(x, x, 0x0000);
			//Displays_drawPixel(x, x, 0x07e0);
		}
	usleep(1000000);
	}
}
