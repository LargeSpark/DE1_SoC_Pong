#include "pongSprites.h"

volatile int sprite_Ball[17][17];

int paddlesize_x = 5;
int paddlesize_y = 60;

//This function initialises the ball
void pongSprites_initBall(void){
	int x = 8;
	int y = 8;
	//Radius as signed int
	int signedr = 8;
	//Radius squared
	int rad2 = signedr * signedr;
	//Outline threshold
	//Go through x's
	int xc = 0;
	int yc = 0;
	//Loop through all X and Y of square the size of radius squared
	for (xc = -signedr; xc <= signedr; xc++) {
		for (yc = -signedr; yc <= signedr; yc++) {
			//radius squared = yc^2 + xc^2
			int pyr = (yc*yc) + (xc*xc);
			//If no fill then draw outline
			if(pyr <= rad2){
				sprite_Ball[xc+x][yc+y] = 1;
			}
			else{
				sprite_Ball[xc+x][yc+y] = 0;
			}
		}
	}
}

//This function renders the ball
void pongSprites_renderBall(int x, int y, short colour){
	int yl;
	int xl;
	//adjust so called at centre of ball
	x = x-8;
	y = y-8;
	//loop through values and check if they equal radius
	for(yl = 0; yl < 17; yl++){
		for(xl = 0; xl < 17; xl++){
			if(sprite_Ball[yl][xl] == 1){
				Displays_setPixel(x+xl, y+yl, colour); //set pixels
			}
		}
	}
}

//This function renders a paddle
void pongSprites_renderPaddle(int x, int y, short colour){
	//taken from assignment 1 - graphics library and modified for paddle
	int lly;
	int llx;
	//adjust for centre
	x = x - (paddlesize_x/2);
	y = y - (paddlesize_y/2);
	//loop through values and set pixels
	for(lly=0; lly <= paddlesize_y; lly++){
		for(llx=0; llx<=paddlesize_x; llx++){
			Displays_setPixel(x+llx,y+lly,colour); //set pixels
		}
	}
}

//This function renders a rectangle
void pongSprites_renderRectangle(int x1,int x2, int y1, int y2, int colour){
	//taken from assignment 1 - graphics library and modified for paddle
	int height = abs(y2-y1);
	int width = abs(x1-x2);
	int y=0;
	int x=0;

	//find bottom left value
	int llx = 0;
	int lly = 0;
	if(x1<x2){
		llx = x1;
	}
	else{
		llx = x2;
	}
	if(y1<y2){
		lly = y1;
	}
	else{
		lly = y2;
	}
	//set pixels
	for(y=0; y <= height; y++){
		for(x=0; x<=width; x++){
			Displays_setPixel(x+llx,y+lly,colour);
		}
	}
}

//This function writes text to the screen
void pongSprites_writeText(int x, int y, int size, char *text, short colour){
	int letter[5][7];	//letter 2 dimensional array
	int stringLength = strlen(text);	//string length
	int character;	//current character
	int line; //current line
	int pixel; //current pixel
	int xp , yp; //counter for rendering of character
	//for each character
	for(character = 0; character < stringLength; character++){
		int charX; //char value
		if(size == 0){charX = x+((character)*6);} //if small
		if(size == 1){charX = x+((character)*12);} //if large
		//This code essentially turns the array 90 degrees and stores value in the letter 2-D array
		//for each line
		for(line = 0; line < 5; line++){
			//for each pixel
			for(pixel = 0; pixel < 7; pixel++){
				//check if pixel is on for the current character
				int pixelon = BF_fontMap[text[character] - ' '][line];
				int shiftedbit = 0;
				//shift down for each pixel to go down the line of pixels
				shiftedbit = (pixelon >> pixel) & 1;
				//output to letter array
				letter[line][pixel] = shiftedbit;
			}
		}
		//render character
		for(yp = 0; yp < 7; yp++){
			for(xp = 0; xp < 5; xp++){
				//if pixel is on
				if(letter[xp][yp] == 1){
					if(size == 0){Displays_setPixel(charX+xp, y+yp, colour);} //if small set 1 pixel
					if(size == 1){ //if large set 4 pixels per pixel
						Displays_setPixel(charX+(xp*2), y+(yp*2), colour);
						Displays_setPixel(charX+(xp*2)+1, y+(yp*2), colour);
						Displays_setPixel(charX+(xp*2), y+(yp*2)+1, colour);
						Displays_setPixel(charX+(xp*2)+1, y+(yp*2)+1, colour);
					}
				}
			}
		}
	}
}

//This function changes the paddles size
void pongSprites_changePaddleSize(unsigned int size){
	paddlesize_y = 60+(size - 5)*10;
}

//This function gets the paddle size
unsigned int pongSprites_getPaddleSizeY( void ){
	return paddlesize_y;
}
