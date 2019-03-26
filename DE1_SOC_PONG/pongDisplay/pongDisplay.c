#include "pongDisplay.h"

int PixelHeight = 240;
int PixelWidth = 320;
int totalPixel = 76800;
volatile short frontFrameBuffer[320][240];
volatile short rearFrameBuffer[320][240];

void Displays_init(volatile short vga_PixelAddress, unsigned volatile int vga_CharacterAddress, unsigned volatile int lcd_pio_base, unsigned volatile int lcd_hw_base){
	int x = 0;
	int y = 0;
	//init VGA
	VGA_init(vga_PixelAddress,vga_CharacterAddress);
	//init LP24
	LT24_initialise(lcd_pio_base,lcd_hw_base);
	//Set framebuffers black
	for(y = 0; y < PixelHeight; y++){
		for(x = 0; x < PixelWidth; x++){
			frontFrameBuffer[x][y] = 0x000;
			rearFrameBuffer[x][y] = 0x001;
		}
	}
}

void Displays_drawPixel(int x, int y, short colour){
	VGA_drawPixel(x,y, colour);
	LT24_drawPixel(colour,240-y,x);
}

void Displays_clearScreen(){
	VGA_clearScreen();
	LT24_clearDisplay(0x000);
}

void Displays_Refresh(){
	int y = 0;
	int x = 0;
	for(y = 0; y < PixelHeight; y++){
		for(x = 0; x < PixelWidth; x++){
			if(frontFrameBuffer[x][y] != rearFrameBuffer[x][y]){
				VGA_drawPixel(x,y, frontFrameBuffer[x][y]);
				LT24_drawPixel(frontFrameBuffer[x][y],240-y,x);
				rearFrameBuffer[x][y] = frontFrameBuffer[x][y];
			}
		}
	}
}

void Displays_setPixel(int x, int y, short colour){
	frontFrameBuffer[x][y] = colour;
}
