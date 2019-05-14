//This is the layer of code between the game and the display devices. It utalises a frame buffer.
#include "pongDisplay.h"
int PixelHeight = 240; //Screen height
int PixelWidth = 320; //Screen Width
int totalPixel = 76800; //Total Pixels
/*--- Global Variables for frame buffers ---*/
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
int frameskip = 0;
int framecount = 0;
//Safety Window
int minX = 0;
int minY = 0;
int maxX = 320;
int maxY = 240;

//This function initialises the displays by setting the addresses
void Displays_init(volatile int vga_PixelAddress, unsigned volatile int vga_CharacterAddress, unsigned volatile int lcd_pio_base, unsigned volatile int lcd_hw_base){
	//init VGA
	VGA_init(vga_PixelAddress,vga_CharacterAddress);
	//init LP24
	LT24_initialise(lcd_pio_base,lcd_hw_base);
	//Set framebuffers black

}
//This function sets the frameskip amount.
void Displays_frameSkip(int skipamount){
	frameskip = skipamount;
}
//This function sets the display mode and then initialises the values.
void Displays_mode(int mode){
	int x = 0; //counter for x
	int y = 0; //counter for y
	modeSet = mode; //set global variable
	//if single software buffer
	if(modeSet == SOFTWAREFB){
		//for loops to set values to zero in 2 dimensional array
		 for(y = 0; y < PixelHeight; y++){
			 for(x = 0; x < PixelWidth; x++){
				frontFrameBuffer[x][y] = 0x000;
				rearFrameBuffer[x][y] = 0x000;
			 }
		 }
	 }
	//if software buffer split into 4
	 else if(modeSet == SOFTWAREQUADFB || modeSet == FASTFB){
		 //for loops to set values to zero in 2 dimensional array
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
	//if software buffer split into 8
	 else if (modeSet == SOFTWAREOCTOFB){
		 //for loops to set values to zero in 2 dimensional array
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
//This function set the pixel to the hardware
void Displays_drawPixel(int x, int y, short colour){
	VGA_drawPixel(x,y, colour); //Set pixel on VGA hardware
	LT24_drawPixel(colour,240-y,x); //Set pixel on LT24 hardware
	ResetWDT(); //reset watchdog
}

//This function clears the screen
void Displays_clearScreen(){
	VGA_clearScreen(); //clear screen VGA
	LT24_clearDisplay(0x000); //clear screen LT24
	Displays_fillColour(0); //set frame buffer values to 0
}

//This function refreshes the screens from the framebuffer and checks the mode.
void Displays_Refresh(){
	//check the mode
	if(frameskip==0){
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
		else if(modeSet == HARDWAREFB){

		}
	}
	else{
		if(frameskip == framecount){
			framecount = 0;
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
			else if(modeSet == HARDWAREFB){

			}
		}
		else{
			framecount++;
		}
	}
}

//This forces a refresh regardless of the frameskip value
void Displays_forceRefresh(){
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
		else if(modeSet == HARDWAREFB){

		}
		else{
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
			else if(modeSet == HARDWAREFB){

			}
		}
}

//This sets the display window which makes sure that the display can not set outside of these values which prevents errors
void Displays_setWindow(int minx, int miny, int maxx, int maxy){
	minX = minx;
	minY = miny;
	maxX = maxx;
	maxY = maxy;
}

//This sets the pixel to the framebuffer
void Displays_setPixel(int x, int y, short colour){
	//check if within window
	//int z;
	//int found = 0;
	//checks if within window set
	if(x >= minX && x < maxX && y >= minY && y < maxY){
		//if no framebuffer draw straight to the display hardware
		if(modeSet == NOFRAMEBUFFER){
			Displays_drawPixel(x,y,colour);
		}
		//if single framebuffer then set the buffer
		else if(modeSet == SOFTWAREFB){
			frontFrameBuffer[x][y] = colour;
		}
		//if quad framebuffer then check which segment of screen pixel in and set framebuffer
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
		//if octo framebuffer then check which segment of screen pixel in and set framebuffer
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
}

//This gets the pixel set on the display.
short Displays_getPixel(int x, int y){
	short pixel = 0x000;
	if(modeSet == 0){
		pixel = 0x000;
	}
	else if(modeSet == SOFTWAREFB){
		pixel = rearFrameBuffer[x][y];
	}
	else if(modeSet == SOFTWAREQUADFB || modeSet == FASTFB){
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
		if(x<80 && y<120){
			pixel = OCTOfrontFrameBuffer1[x][y];
		}
		else if(x>=80 && y<120 && x<160){
			pixel = OCTOfrontFrameBuffer2[x-80][y];
		}
		else if(x>=160 && y<120 && x<240){
			pixel = OCTOfrontFrameBuffer3[x-160][y];
		}
		else if(x>=240 && y<120){
			pixel = OCTOfrontFrameBuffer4[x-240][y];
		}
		else if(x<80 && y>=120){
			pixel = OCTOfrontFrameBuffer5[x][y-120];
		}
		else if(x>=80 && y>=120 && x<160){
			pixel = OCTOfrontFrameBuffer6[x-80][y-120];
		}
		else if(x>=160 && y>=120 && x<240){
			pixel = OCTOfrontFrameBuffer7[x-160][y-120];
		}
		else if(x>=240 && y>=120){
			pixel = OCTOfrontFrameBuffer8[x-240][y-120];
		}
	}
	return pixel;
}

//FrameBuffer Refreshes
//This function refreshes the octo framebuffer
void DisplaysLocal_octoRefresh(){
	int x; //x counter
	int y; //y counter
	//compare front and rear buffers using memcmp
	int o1change = memcmp(OCTOfrontFrameBuffer1,OCTOrearFrameBuffer1, sizeof(OCTOfrontFrameBuffer1));
	int o2change = memcmp(OCTOfrontFrameBuffer2,OCTOrearFrameBuffer2, sizeof(OCTOfrontFrameBuffer2));
	int o3change = memcmp(OCTOfrontFrameBuffer3,OCTOrearFrameBuffer3, sizeof(OCTOfrontFrameBuffer3));
	int o4change = memcmp(OCTOfrontFrameBuffer4,OCTOrearFrameBuffer4, sizeof(OCTOfrontFrameBuffer4));
	int o5change = memcmp(OCTOfrontFrameBuffer5,OCTOrearFrameBuffer5, sizeof(OCTOfrontFrameBuffer5));
	int o6change = memcmp(OCTOfrontFrameBuffer6,OCTOrearFrameBuffer6, sizeof(OCTOfrontFrameBuffer6));
	int o7change = memcmp(OCTOfrontFrameBuffer7,OCTOrearFrameBuffer7, sizeof(OCTOfrontFrameBuffer7));
	int o8change = memcmp(OCTOfrontFrameBuffer8,OCTOrearFrameBuffer8, sizeof(OCTOfrontFrameBuffer8));
	//if any changes whilst running through loop then update frame buffer and set on hardware.
	for(y = 0; y < PixelHeight/2; y++){
		for(x = 0; x < PixelWidth/4; x++){
			if(o1change != 0){
				if(OCTOfrontFrameBuffer1[x][y] != OCTOrearFrameBuffer1[x][y]){
					Displays_drawPixel(x,y,OCTOfrontFrameBuffer1[x][y]); //set on hardware
					OCTOrearFrameBuffer1[x][y] = OCTOfrontFrameBuffer1[x][y]; //update FB
				}
			}
			if(o2change != 0){
				if(OCTOfrontFrameBuffer2[x][y] != OCTOrearFrameBuffer2[x][y]){
					Displays_drawPixel(x+80,y,OCTOfrontFrameBuffer2[x][y]); //set on hardware
					OCTOrearFrameBuffer2[x][y] = OCTOfrontFrameBuffer2[x][y]; //update FB
				}
			}
			if(o3change != 0){
				if(OCTOfrontFrameBuffer3[x][y] != OCTOrearFrameBuffer3[x][y]){
					Displays_drawPixel(x+160,y,OCTOfrontFrameBuffer3[x][y]); //set on hardware
					OCTOrearFrameBuffer3[x][y] = OCTOfrontFrameBuffer3[x][y]; //update FB
				}
			}
			if(o4change != 0){
				if(OCTOfrontFrameBuffer4[x][y] != OCTOrearFrameBuffer4[x][y]){
					Displays_drawPixel(x+240,y,OCTOfrontFrameBuffer4[x][y]); //set on hardware
					OCTOrearFrameBuffer4[x][y] = OCTOfrontFrameBuffer4[x][y];  //update FB
				}
			}
			if(o5change != 0){
				if(OCTOfrontFrameBuffer5[x][y] != OCTOrearFrameBuffer5[x][y]){
					Displays_drawPixel(x,y+120,OCTOfrontFrameBuffer5[x][y]); //set on hardware
					OCTOrearFrameBuffer5[x][y] = OCTOfrontFrameBuffer5[x][y]; //update FB
				}
			}
			if(o6change != 0){
				if(OCTOfrontFrameBuffer6[x][y] != OCTOrearFrameBuffer6[x][y]){
					Displays_drawPixel(x+80,y+120,OCTOfrontFrameBuffer6[x][y]); //set on hardware
					OCTOrearFrameBuffer6[x][y] = OCTOfrontFrameBuffer6[x][y]; //update FB
				}
			}
			if(o7change != 0){
				if(OCTOfrontFrameBuffer7[x][y] != OCTOrearFrameBuffer7[x][y]){
					Displays_drawPixel(x+160,y+120,OCTOfrontFrameBuffer7[x][y]); //set on hardware
					OCTOrearFrameBuffer7[x][y] = OCTOfrontFrameBuffer7[x][y]; //update FB
				}
			}
			if(o8change != 0){
				if(OCTOfrontFrameBuffer8[x][y] != OCTOrearFrameBuffer8[x][y]){
					Displays_drawPixel(x+240,y+120,OCTOfrontFrameBuffer8[x][y]); //set on hardware
					OCTOrearFrameBuffer8[x][y] = OCTOfrontFrameBuffer8[x][y]; //update FB
				}
			}
		}
		}

}

//This function refreshes the quad framebuffer
void DisplaysLocal_quadRefresh(){
	int x; //x counter
	int y; //y counter
	//compare front and rear buffers using memcmp
	int q1change = memcmp(frontFrameBuffer1,rearFrameBuffer1, sizeof(frontFrameBuffer1));
	int q2change = memcmp(frontFrameBuffer2,rearFrameBuffer2, sizeof(frontFrameBuffer2));
	int q3change = memcmp(frontFrameBuffer3,rearFrameBuffer3, sizeof(frontFrameBuffer3));
	int q4change = memcmp(frontFrameBuffer4,rearFrameBuffer4, sizeof(frontFrameBuffer4));
	//if any changes whilst running through loop then update frame buffer and set on hardware.
	for(y = 0; y < PixelHeight/2; y++){
			for(x = 0; x < PixelWidth/2; x++){
				if(q1change != 0){
					if(frontFrameBuffer1[x][y] != rearFrameBuffer1[x][y]){
						Displays_drawPixel(x,y,frontFrameBuffer1[x][y]); //set on hardware
						rearFrameBuffer1[x][y] = frontFrameBuffer1[x][y]; //update FB
					}
				}
				if(q2change != 0){
					if(frontFrameBuffer2[x][y] != rearFrameBuffer2[x][y]){
						Displays_drawPixel(x+160,y,frontFrameBuffer2[x][y]); //set on hardware
						rearFrameBuffer2[x][y] = frontFrameBuffer2[x][y]; //update FB
					}
				}
				if(q3change != 0){
					if(frontFrameBuffer3[x][y] != rearFrameBuffer3[x][y]){
						Displays_drawPixel(x,y+120,frontFrameBuffer3[x][y]); //set on hardware
						rearFrameBuffer3[x][y] = frontFrameBuffer3[x][y]; //update FB
					}
				}
				if(q4change != 0){
					if(frontFrameBuffer4[x][y] != rearFrameBuffer4[x][y]){
						Displays_drawPixel(x+160,y+120,frontFrameBuffer4[x][y]); //set on hardware
						rearFrameBuffer4[x][y] = frontFrameBuffer4[x][y]; //update FB
					}
				}
			}
	}
}
//This function find changes and sets them in hardware.
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
//This function fills the displays a set colour.
void Displays_fillColour(short colour){
	int x;
	int y;
	VGA_fillColour(colour);
	LT24_clearDisplay(colour);
	for(y = 0; y < 240; y++){
		for(x = 0; x < 320; x++){
			Displays_setPixel(x,y,colour);
		}
	}
}
