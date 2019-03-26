#include "pongDisplay.h"
int PixelHeight = 240;
int PixelWidth = 320;
int totalPixel = 76800;
//Single framebuffer
volatile short frontFrameBuffer[320][240];
volatile short rearFrameBuffer[320][240];
//Quad framebuffer
volatile short frontFrameBuffer1[80][60];
volatile short rearFrameBuffer1[80][60];
volatile short frontFrameBuffer2[80][60];
volatile short rearFrameBuffer2[80][60];
volatile short frontFrameBuffer3[80][60];
volatile short rearFrameBuffer3[80][60];
volatile short frontFrameBuffer4[80][60];
volatile short rearFrameBuffer4[80][60];
//defines
int modeSet = 1;


void Displays_init(volatile short vga_PixelAddress, unsigned volatile int vga_CharacterAddress, unsigned volatile int lcd_pio_base, unsigned volatile int lcd_hw_base){

	//init VGA
	VGA_init(vga_PixelAddress,vga_CharacterAddress);
	//init LP24
	LT24_initialise(lcd_pio_base,lcd_hw_base);
	//Set framebuffers black

}

void Displays_mode(int mode){
	int x = 0;
	int y = 0;
	modeSet = mode;
	if(modeSet == 1){
		 for(y = 0; y < PixelHeight; y++){
			 for(x = 0; x < PixelWidth; x++){
				frontFrameBuffer[x][y] = 0x000;
				rearFrameBuffer[x][y] = 0x000;
			 }
		 }
 }
 else if(modeSet == 2){
	 for(y = 0; y < (PixelHeight/4); y++){
		 for(x = 0; x < (PixelWidth/4); x++){
			 frontFrameBuffer1[80][60] = 0x000;
			 rearFrameBuffer1[80][60] = 0x000;
			 frontFrameBuffer2[80][60] = 0x000;
			 rearFrameBuffer2[80][60] = 0x000;
			 frontFrameBuffer3[80][60] = 0x000;
			 rearFrameBuffer3[80][60] = 0x000;
			 frontFrameBuffer4[80][60] = 0x000;
			 rearFrameBuffer4[80][60] = 0x000;
		 }
	 }
 }
 else if (modeSet == 3){

  }
}

void Displays_drawPixel(int x, int y, short colour){
	VGA_drawPixel(x,y, colour);
	LT24_drawPixel(colour,240-y,x);
	ResetWDT();
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
			if(modeSet == 1){
				short colour = frontFrameBuffer[x][y];
				if(frontFrameBuffer[x][y] != rearFrameBuffer[x][y]){
					short colour = frontFrameBuffer[x][y];
					Displays_drawPixel(x,y,colour);
					rearFrameBuffer[x][y] = colour;
				}
			}
			else if(modeSet == 2){
				if(frontFrameBuffer1[x][y] != rearFrameBuffer1[x][y]){
					Displays_drawPixel(x,y,frontFrameBuffer1[x][y]);
					rearFrameBuffer1[x][y] = frontFrameBuffer1[x][y];
				}
				if(frontFrameBuffer2[x][y] != rearFrameBuffer2[x][y]){
					Displays_drawPixel(x+80,y,frontFrameBuffer2[x][y]);
					rearFrameBuffer2[x][y] = frontFrameBuffer2[x][y];
				}
				if(frontFrameBuffer3[x][y] != rearFrameBuffer3[x][y]){
					Displays_drawPixel(x,y+60,frontFrameBuffer3[x][y]);
					rearFrameBuffer3[x][y] = frontFrameBuffer3[x][y];
				}
				if(frontFrameBuffer4[x][y] != rearFrameBuffer4[x][y]){
					Displays_drawPixel(x+80,y+60,frontFrameBuffer4[x][y]);
					rearFrameBuffer4[x][y] = frontFrameBuffer4[x][y];
				}
				}
			}
	}
}

void Displays_setPixel(int x, int y, short colour){
	if(modeSet == 1){
	frontFrameBuffer[x][y] = colour;
	ResetWDT();
	}
	else if(modeSet == 2){
		if(x<80 && y<60){
			frontFrameBuffer1[x][y] = colour;
		}
		else if(x>=80 && y<60){
			frontFrameBuffer2[x-80][y] = colour;
		}
		else if(x<80 && y>=60){
			frontFrameBuffer3[x][y-60] = colour;
		}
		else if(x>=80 && y>=60){
			frontFrameBuffer4[x-80][y-60] = colour;
		}
	}
}
