#include "DE1SoC_VGA.h"

//http://www-ug.eecg.utoronto.ca/msl/nios_devices_SoC/dev_vga.html
//320x240 pixel resolution
unsigned volatile short *vga_addr = 0x0; //0x08000000

void VGA_init(unsigned volatile short vga_address){
	vga_addr = (unsigned short *) vga_address;
}

void VGA_drawPixel(int x, int y, short colour){
	volatile short *vga_addr=(volatile short*)(vga_addr + (y<<10) + (x<<1));
	//volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
	*vga_addr=colour;
}

void VGA_clearScreen(){
	  int x, y;
	  for (x = 0; x < 320; x++) {
	    for (y = 0; y < 240; y++) {
	    	VGA_drawPixel(x,y,0);
		}
	  }
}
