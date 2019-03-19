#include "DE1SoC_VGA.h"

//http://www-ug.eecg.utoronto.ca/msl/nios_devices_SoC/dev_vga.html
//http://www-ug.eecg.toronto.edu/msl/handouts/DE1-SoC_Computer_Nios.pdf#page=34
//320x240 pixel resolution
//volatile short *vga_addr = 0x08000000; //0x08000000

//volatile unsigned int *vga_charaddr = 0x0; //0x09000000

void VGA_init(unsigned int vgaPixelBuffer_address, unsigned int vgaCharBuffer_address){
	//vga_addr = (short *) vgaPixelBuffer_address;
	//vga_charaddr = (unsigned int *) vgaCharBuffer_address;
}

void VGA_drawPixel(int x, int y, short colour){
	//volatile short *vga_addr=(volatile short*)(vga_addr + (y<<10) + (x<<1));
	volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
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
