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
//OCTO framebuffer
volatile short OCTOfrontFrameBuffer1[80][120];
volatile short OCTOrearFrameBuffer1[80][120];
volatile short OCTOfrontFrameBuffer2[80][120];
volatile short OCTOrearFrameBuffer2[80][120];
volatile short OCTOfrontFrameBuffer3[80][120];
volatile short OCTOrearFrameBuffer3[80][120];
volatile short OCTOfrontFrameBuffer4[80][120];
volatile short OCTOrearFrameBuffer4[80][120];
volatile short OCTOfrontFrameBuffer5[80][120];
volatile short OCTOrearFrameBuffer5[80][120];
volatile short OCTOfrontFrameBuffer6[80][120];
volatile short OCTOrearFrameBuffer6[80][120];
volatile short OCTOfrontFrameBuffer7[80][120];
volatile short OCTOrearFrameBuffer7[80][120];
volatile short OCTOfrontFrameBuffer8[80][120];
volatile short OCTOrearFrameBuffer8[80][120];
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
	if(modeSet == SOFTWAREFB){
		 for(y = 0; y < PixelHeight; y++){
			 for(x = 0; x < PixelWidth; x++){
				frontFrameBuffer[x][y] = 0x000;
				rearFrameBuffer[x][y] = 0x000;
			 }
		 }
 }
 else if(modeSet == SOFTWAREQUADFB){
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
 else if (modeSet == SOFTWAREOCTOFB){
	 for(y = 0; y < 120; y++){
	 		 for(x = 0; x < 160; x++){
	 			 OCTOfrontFrameBuffer1[x][y] = 0x000;
	 			 OCTOrearFrameBuffer1[x][y] = 0x000;
	 			 OCTOfrontFrameBuffer2[x][y] = 0x000;
	 			 OCTOrearFrameBuffer2[x][y] = 0x000;
	 			 OCTOfrontFrameBuffer3[x][y] = 0x000;
	 			 OCTOrearFrameBuffer3[x][y] = 0x000;
	 			 OCTOfrontFrameBuffer4[x][y] = 0x000;
	 			 OCTOrearFrameBuffer4[x][y] = 0x000;
	 			 OCTOfrontFrameBuffer5[x][y] = 0x000;
	 			 OCTOrearFrameBuffer5[x][y] = 0x000;
	 			 OCTOfrontFrameBuffer6[x][y] = 0x000;
	 			 OCTOrearFrameBuffer6[x][y] = 0x000;
	 			 OCTOfrontFrameBuffer7[x][y] = 0x000;
	 			 OCTOrearFrameBuffer7[x][y] = 0x000;
	 			 OCTOfrontFrameBuffer8[x][y] = 0x000;
	 			 OCTOrearFrameBuffer8[x][y] = 0x000;
	 		 }
	 	 }
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
	else if(modeSet == SOFTWAREFB){
		DisplaysLocal_singleRefresh();
	}
	else if(modeSet == SOFTWAREQUADFB){
		DisplaysLocal_quadRefresh();
	}
	else if(modeSet == SOFTWAREOCTOFB){
		DisplaysLocal_octoRefresh();
	}
}

void Displays_setPixel(int x, int y, short colour){
	if(modeSet == NOFRAMEBUFFER){
		Displays_drawPixel(x,y,colour);
	}
	else if(modeSet == SOFTWAREFB){
		frontFrameBuffer[x][y] = colour;
		ResetWDT();
	}
	else if(modeSet == SOFTWAREQUADFB){
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
	else if(modeSet == SOFTWAREOCTOFB){
		if(x<80 && y<120){
			OCTOfrontFrameBuffer1[x][y] = colour;
		}
		else if(x>=80 && y<120 && x<160){
			OCTOfrontFrameBuffer2[x-80][y] = colour;
		}
		else if(x>=160 && y<120 && x<240){
			OCTOfrontFrameBuffer3[x-160][y] = colour;
		}
		else if(x>=240 && y<120){
			OCTOfrontFrameBuffer4[x-240][y] = colour;
		}
		else if(x<80 && y>=120){
			OCTOfrontFrameBuffer5[x][y-120] = colour;
		}
		else if(x>=80 && y>=120 && x<160){
			OCTOfrontFrameBuffer6[x-80][y-120] = colour;
		}
		else if(x>=160 && y>=120 && x<240){
			OCTOfrontFrameBuffer7[x-160][y-120] = colour;
		}
		else if(x>=240 && y>=120){
			OCTOfrontFrameBuffer8[x-240][y-120] = colour;
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
	else if(modeSet == SOFTWAREOCTOFB){

	}
	return pixel;
}

//FrameBuffer Refreshes
void DisplaysLocal_octoRefresh(){
	int x;
	int y;
	int o1change = memcmp(OCTOfrontFrameBuffer1,OCTOrearFrameBuffer1, sizeof(OCTOfrontFrameBuffer1));
	int o2change = memcmp(OCTOfrontFrameBuffer2,OCTOrearFrameBuffer2, sizeof(OCTOfrontFrameBuffer2));
	int o3change = memcmp(OCTOfrontFrameBuffer3,OCTOrearFrameBuffer3, sizeof(OCTOfrontFrameBuffer3));
	int o4change = memcmp(OCTOfrontFrameBuffer4,OCTOrearFrameBuffer4, sizeof(OCTOfrontFrameBuffer4));
	int o5change = memcmp(OCTOfrontFrameBuffer5,OCTOrearFrameBuffer5, sizeof(OCTOfrontFrameBuffer5));
	int o6change = memcmp(OCTOfrontFrameBuffer6,OCTOrearFrameBuffer6, sizeof(OCTOfrontFrameBuffer6));
	int o7change = memcmp(OCTOfrontFrameBuffer7,OCTOrearFrameBuffer7, sizeof(OCTOfrontFrameBuffer7));
	int o8change = memcmp(OCTOfrontFrameBuffer8,OCTOrearFrameBuffer8, sizeof(OCTOfrontFrameBuffer8));
	for(y = 0; y < PixelHeight/2; y++){
		for(x = 0; x < PixelWidth/4; x++){
			if(o1change != 0){
				if(OCTOfrontFrameBuffer1[x][y] != OCTOrearFrameBuffer1[x][y]){
					Displays_drawPixel(x,y,OCTOfrontFrameBuffer1[x][y]);
					OCTOrearFrameBuffer1[x][y] = OCTOfrontFrameBuffer1[x][y];
				}
			}
			if(o2change != 0){
				if(OCTOfrontFrameBuffer2[x][y] != OCTOrearFrameBuffer2[x][y]){
					Displays_drawPixel(x+80,y,OCTOfrontFrameBuffer2[x][y]);
					OCTOrearFrameBuffer2[x][y] = OCTOfrontFrameBuffer2[x][y];
				}
			}
			if(o3change != 0){
				if(OCTOfrontFrameBuffer3[x][y] != OCTOrearFrameBuffer3[x][y]){
					Displays_drawPixel(x+160,y,OCTOfrontFrameBuffer3[x][y]);
					OCTOrearFrameBuffer3[x][y] = OCTOfrontFrameBuffer3[x][y];
				}
			}
			if(o4change != 0){
				if(OCTOfrontFrameBuffer4[x][y] != OCTOrearFrameBuffer4[x][y]){
					Displays_drawPixel(x+240,y,OCTOfrontFrameBuffer4[x][y]);
					OCTOrearFrameBuffer4[x][y] = OCTOfrontFrameBuffer4[x][y];
				}
			}
			if(o5change != 0){
				if(OCTOfrontFrameBuffer5[x][y] != OCTOrearFrameBuffer5[x][y]){
					Displays_drawPixel(x,y+120,OCTOfrontFrameBuffer5[x][y]);
					OCTOrearFrameBuffer5[x][y] = OCTOfrontFrameBuffer5[x][y];
				}
			}
			if(o6change != 0){
				if(OCTOfrontFrameBuffer6[x][y] != OCTOrearFrameBuffer6[x][y]){
					Displays_drawPixel(x+80,y+120,OCTOfrontFrameBuffer6[x][y]);
					OCTOrearFrameBuffer6[x][y] = OCTOfrontFrameBuffer6[x][y];
				}
			}
			if(o7change != 0){
				if(OCTOfrontFrameBuffer7[x][y] != OCTOrearFrameBuffer7[x][y]){
					Displays_drawPixel(x+160,y+120,OCTOfrontFrameBuffer7[x][y]);
					OCTOrearFrameBuffer7[x][y] = OCTOfrontFrameBuffer7[x][y];
				}
			}
			if(o8change != 0){
				if(OCTOfrontFrameBuffer8[x][y] != OCTOrearFrameBuffer8[x][y]){
					Displays_drawPixel(x+240,y+120,OCTOfrontFrameBuffer8[x][y]);
					OCTOrearFrameBuffer8[x][y] = OCTOfrontFrameBuffer8[x][y];
				}
			}
		}
		}

}

void DisplaysLocal_quadRefresh(){
	int x;
	int y;
	int q1change = memcmp(frontFrameBuffer1,rearFrameBuffer1, sizeof(frontFrameBuffer1));
	int q2change = memcmp(frontFrameBuffer2,rearFrameBuffer2, sizeof(frontFrameBuffer2));
	int q3change = memcmp(frontFrameBuffer3,rearFrameBuffer3, sizeof(frontFrameBuffer3));
	int q4change = memcmp(frontFrameBuffer4,rearFrameBuffer4, sizeof(frontFrameBuffer4));
	for(y = 0; y < PixelHeight/2; y++){
			for(x = 0; x < PixelWidth/2; x++){
				if(q1change != 0){
					if(frontFrameBuffer1[x][y] != rearFrameBuffer1[x][y]){
						Displays_drawPixel(x,y,frontFrameBuffer1[x][y]);
						rearFrameBuffer1[x][y] = frontFrameBuffer1[x][y];
					}
				}
				if(q2change != 0){
					if(frontFrameBuffer2[x][y] != rearFrameBuffer2[x][y]){
						Displays_drawPixel(x+160,y,frontFrameBuffer2[x][y]);
						rearFrameBuffer2[x][y] = frontFrameBuffer2[x][y];
					}
				}
				if(q3change != 0){
					if(frontFrameBuffer3[x][y] != rearFrameBuffer3[x][y]){
						Displays_drawPixel(x,y+120,frontFrameBuffer3[x][y]);
						rearFrameBuffer3[x][y] = frontFrameBuffer3[x][y];
					}
				}
				if(q4change != 0){
					if(frontFrameBuffer4[x][y] != rearFrameBuffer4[x][y]){
						Displays_drawPixel(x+160,y+120,frontFrameBuffer4[x][y]);
						rearFrameBuffer4[x][y] = frontFrameBuffer4[x][y];
					}
				}
			}
	}
}

void DisplaysLocal_singleRefresh(){
	int x;
	int y;
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
