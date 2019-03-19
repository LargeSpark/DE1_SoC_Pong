#include "DE1Soc_VGA/DE1SoC_VGA.h"

int main(void){
//VGA TESTCODE
		int x;
		VGA_init(0x08000000);
		VGA_clearScreen();

	   for (x=0;x<320;x++)
	   {
	      // Draw a straight line in red across the screen centre
		   VGA_drawPixel(x, 59, 0xf800);
	      // Draw a "diagonal" line in green
	      if (x<240)
	    	  VGA_drawPixel(x, x, 0x07e0);
	   }
	   return 0;
}
