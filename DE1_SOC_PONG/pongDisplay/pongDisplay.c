#include "pongDisplay.h"
int PixelHeight = 240;
int PixelWidth = 320;
int totalPixel = 76800;
//Single framebuffer
volatile short frontFrameBuffer[320][240];
volatile short rearFrameBuffer[320][240];
//Quad framebuffer
volatile short frontFrameBuffer1[160][120];
volatile short rearFrameBuffer1[160][120];
volatile short frontFrameBuffer2[160][120];
volatile short rearFrameBuffer2[160][120];
volatile short frontFrameBuffer3[160][120];
volatile short rearFrameBuffer3[160][120];
volatile short frontFrameBuffer4[160][120];
volatile short rearFrameBuffer4[160][120];
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
	 for(y = 0; y < 120; y++){
		 for(x = 0; x < 160; x++){
			 frontFrameBuffer1[x][y] = 0x000;
			 rearFrameBuffer1[x][y] = 0x000;
			 frontFrameBuffer2[x][y] = 0x000;
			 rearFrameBuffer2[x][y] = 0x000;
			 frontFrameBuffer3[x][y] = 0x000;
			 rearFrameBuffer3[x][y] = 0x000;
			 frontFrameBuffer4[x][y] = 0x000;
			 rearFrameBuffer4[x][y] = 0x000;
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
	if(modeSet == 0){

	}
	else if(modeSet == 1){
	for(y = 0; y < PixelHeight; y++){
		for(x = 0; x < PixelWidth; x++){
			short colour = frontFrameBuffer[x][y];
				if(frontFrameBuffer[x][y] != rearFrameBuffer[x][y]){
					short colour = frontFrameBuffer[x][y];
					Displays_drawPixel(x,y,colour);
					rearFrameBuffer[x][y] = colour;
				}
			}

		}
	}
	else if(modeSet == 2){
		for(y = 0; y < PixelHeight/2; y++){
				for(x = 0; x < PixelWidth/2; x++){
					if(frontFrameBuffer1[x][y] != rearFrameBuffer1[x][y]){
						Displays_drawPixel(x,y,frontFrameBuffer1[x][y]);
						rearFrameBuffer1[x][y] = frontFrameBuffer1[x][y];
					}
					if(frontFrameBuffer2[x][y] != rearFrameBuffer2[x][y]){
						Displays_drawPixel(x+160,y,frontFrameBuffer2[x][y]);
						rearFrameBuffer2[x][y] = frontFrameBuffer2[x][y];
					}
					if(frontFrameBuffer3[x][y] != rearFrameBuffer3[x][y]){
						Displays_drawPixel(x,y+120,frontFrameBuffer3[x][y]);
						rearFrameBuffer3[x][y] = frontFrameBuffer3[x][y];
					}
					if(frontFrameBuffer4[x][y] != rearFrameBuffer4[x][y]){
						Displays_drawPixel(x+160,y+120,frontFrameBuffer4[x][y]);
						rearFrameBuffer4[x][y] = frontFrameBuffer4[x][y];
					}
				}
		}
	}
}

void Displays_setPixel(int x, int y, short colour){
	if(modeSet == 0){
		Displays_drawPixel(x,y,colour);
	}
	else if(modeSet == 1){
		frontFrameBuffer[x][y] = colour;
		ResetWDT();
	}
	else if(modeSet == 2){
		if(x<160 && y<120){
			frontFrameBuffer1[x][y] = colour;
		}
		else if(x>=160 && y<120){
			frontFrameBuffer2[x-160][y] = colour;
		}
		else if(x<160 && y>=120){
			frontFrameBuffer3[x][y-120] = colour;
		}
		else if(x>=160 && y>=120){
			frontFrameBuffer4[x-160][y-120] = colour;
		}
	}
}

short Displays_getPixel(int x, int y){
	short pixel = 0x000;
	if(modeSet == 0){
		pixel = 0x000;
	}
	else if(modeSet == SOFTWAREFB){
		pixel = rearFrameBuffer[x][y];
	}
	else if(modeSet == SOFTWAREQUADFB){
		if(x<160 && y<120){
			pixel = frontFrameBuffer1[x][y];
		}
		else if(x>=160 && y<120){
			pixel = frontFrameBuffer2[x-160][y];
		}
		else if(x<160 && y>=120){
			pixel = frontFrameBuffer3[x][y-120];
		}
		else if(x>=160 && y>=120){
			pixel = frontFrameBuffer4[x-160][y-120];
		}
	}
	return pixel;
}
