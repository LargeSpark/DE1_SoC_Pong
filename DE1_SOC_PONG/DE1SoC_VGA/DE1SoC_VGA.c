#include "DE1SoC_VGA.h"

//http://www-ug.eecg.utoronto.ca/msl/nios_devices_SoC/dev_vga.html
//http://www-ug.eecg.toronto.edu/msl/handouts/DE1-SoC_Computer_Nios.pdf#page=34
//320x240 pixel resolution
//volatile short *vga_addr = 0x08000000; //0x08000000

short vga_address = 0x0; 	//0xC8000000
short vga_charaddr = 0x0; 	//0xC9000000

//Initialise addresses of VGA
void VGA_init(short vgaPixelBuffer_address, short vgaCharBuffer_address){
	vga_address = vgaPixelBuffer_address;
	vga_charaddr = vgaCharBuffer_address;
	//Clear VGA
	VGA_clearScreen();
}

void VGA_drawPixel(int x, int y, short colour){
	//Call address of pixel
	volatile short *vga_addr=(volatile short*)(vga_address + (y<<10) + (x<<1));
	*vga_addr=colour; //Set pixel to colour
}

void VGA_clearScreen(){
	  int x, y;
	  for (x = 0; x < 320; x++) {
	    for (y = 0; y < 240; y++) {
	    	//set all pixels to black
	    	VGA_drawPixel(x,y,0x0000);
		}
	  }
}
