#include "DE1SoC_VGA.h"


volatile short vga_addr;

void VGA_init(unsigned volatile int VGA){
	vga_addr = (volatile short*)(0x08000000);
}

void VGA_drawPixel(int x, int y, short colour){
	volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
	*vga_addr=colour;
}

void VGA_clearScreen(int x, int y, short colour){

}
