#include "pongDisplay.h"

void Displays_init(unsigned volatile short vga_address,unsigned volatile int lcd_pio_base, unsigned volatile int lcd_hw_base){
	//init VGA
	VGA_init(vga_address);
	//init LP24
	LT24_initialise(lcd_pio_base,lcd_hw_base);
}

void Displays_drawPixel(int x, int y, short colour){
	VGA_drawPixel(x,y, colour);
	LT24_drawPixel(colour,y,x);
}

void Displays_clearScreen(){

}
