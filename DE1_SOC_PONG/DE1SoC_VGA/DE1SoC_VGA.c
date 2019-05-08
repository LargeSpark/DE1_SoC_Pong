#include "DE1SoC_VGA.h"

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
//This function gets the address of a pixel and sets it to a colour
void VGA_drawPixel(int x, int y, short colour){
	//Call address of pixel
    //Address base + [Pixel Y][Pixel X]0
	volatile short *vga_addr=(volatile short*)(0xC8000000 + (y<<10) + (x<<1));
	*vga_addr=colour; //Set pixel to colour
}
//This function sets all pixels to black
void VGA_clearScreen(){
	  int x, y;
	  for (x = 0; x < 320; x++) {
	    for (y = 0; y < 240; y++) {
	    	//set all pixels to black
	    	VGA_drawPixel(x,y,0x0000);
		}
	  }
}
//This function sets all pixels to the desired colour
void VGA_fillColour(short colour){
	  int x, y;
	  for (x = 0; x < 320; x++) {
	    for (y = 0; y < 240; y++) {
	    	//set all pixels to colour
	    	VGA_drawPixel(x,y,colour);
		}
	  }
}
