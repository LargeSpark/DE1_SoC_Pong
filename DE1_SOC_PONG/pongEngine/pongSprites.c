#include "pongSprites.h"

volatile int sprite_Ball[17][17];

int paddlesize_x;
int paddlesize_y;

void pongSprites_init(){
	pongSprites_initBall();
	paddlesize_x = 5;
	paddlesize_y = 60;
}

void pongSprites_initBall(){
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

void pongSprites_renderBall(int x, int y, short colour){
	int yl;
	int xl;
	for(yl = 0; yl < 17; yl++){
		for(xl = 0; xl < 17; xl++){
			if(sprite_Ball[yl][xl] == 1){
				Displays_setPixel(x+xl, y+yl, colour);
			}
		}
	}
}

void pongSprites_renderPaddle(int x, int y, short colour){
	//taken from assignment 1 - graphics library
	int lly;
	int llx;
	for(lly=0; lly <= paddlesize_y; lly++){
		for(llx=0; llx<=paddlesize_x; llx++){
			Displays_setPixel(x+llx,y+lly,colour);
		}
	}
}
