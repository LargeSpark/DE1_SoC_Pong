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
	//adjust so called at centre of ball
	x = x-8;
	y = y-8;

	for(yl = 0; yl < 17; yl++){
		for(xl = 0; xl < 17; xl++){
			if(sprite_Ball[yl][xl] == 1){
				Displays_setPixel(x+xl, y+yl, colour);
			}
		}
	}
}

void pongSprites_renderPaddle(int x, int y, short colour){
	//taken from assignment 1 - graphics library and modified for paddle
	int lly;
	int llx;
	//adjust for centre
	x = x - (paddlesize_x/2);
	y = y - (paddlesize_y/2);
	for(lly=0; lly <= paddlesize_y; lly++){
		for(llx=0; llx<=paddlesize_x; llx++){
			Displays_setPixel(x+llx,y+lly,colour);
		}
	}
}

void pongSprites_writeText(int x, int y, int size, char *text, short colour){
	int letter[5][7];
	int stringLength = strlen(text);
	int character;
	int line;
	int pixel;
	int xp , yp;
	for(character = 0; character < stringLength; character++){
		int charX;
		if(size == SMALL){charX = x+((character)*6);}
		if(size == LARGE){charX = x+((character)*12);}
		for(line = 0; line < 5; line++){
			for(pixel = 0; pixel < 7; pixel++){
				int pixelon = BF_fontMap[text[character] - ' '][line];
				int shiftedbit = 0;
				shiftedbit = (pixelon >> pixel) & 1;
				letter[line][pixel] = shiftedbit;
			}
		}
		//render character
		for(yp = 0; yp < 7; yp++){
			for(xp = 0; xp < 5; xp++){
				if(letter[xp][yp] == 1){
					if(size == SMALL){Displays_setPixel(charX+xp, y+yp, colour);}
					if(size == LARGE){
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
