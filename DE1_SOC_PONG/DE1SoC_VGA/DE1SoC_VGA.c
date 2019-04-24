#include "DE1SoC_VGA.h"

//http://www-ug.eecg.utoronto.ca/msl/nios_devices_SoC/dev_vga.html
//http://www-ug.eecg.toronto.edu/msl/handouts/DE1-SoC_Computer_Nios.pdf#page=34
//320x240 pixel resolution
//volatile short *vga_addr = 0x08000000; //0x08000000

short vga_address = 0x0; 	//0xC8000000
short vga_charaddr = 0x0; 	//0xC9000000
volatile int pixel_buffer_start;

//Initialise addresses of VGA
void VGA_init(short vgaPixelBuffer_address, short vgaCharBuffer_address){
	//init backbuffer
	volatile int * pixel_ctrl_ptr = (int *) 0xFF203020;
	*(pixel_ctrl_ptr + 1) = 0xC8000000;
	pixel_buffer_start = *pixel_ctrl_ptr;
	*(pixel_ctrl_ptr + 1) = 0xC0000000;
	vga_address = vgaPixelBuffer_address;
	vga_charaddr = vgaCharBuffer_address;
	//Clear VGA
	VGA_clearScreen();
}

void VGA_drawPixel(int x, int y, short colour){
	//Call address of pixel
	volatile short *vga_addr=(volatile short*)(0xC8000000 + (y<<10) + (x<<1));
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

void VGA_drawPixelToHwBuffer(int x, int y, short colour){
	//Call address of pixel
	volatile short *vga_addr=(volatile short*)(pixel_buffer_start + (y<<10) + (x<<1));
	*vga_addr=colour; //Set pixel to colour
}

void VGA_BufferSwap(){
	volatile short *vga_addr=(volatile short*)(0xFF203020);
	volatile int *status=(volatile int*)(0xFF20302C);
	int stat = 0;
	*vga_addr= 1;
	//wait while vertical sync
	while(stat == 0){
	if((*status & (1<<0))== 0){
		stat = 1;
		}
	}
}

void VGA_fillColour(short colour){
	  int x, y;
	  for (x = 0; x < 320; x++) {
	    for (y = 0; y < 240; y++) {
	    	//set all pixels to colour
	    	VGA_drawPixel(x,y,colour);
		}
	  }
}
