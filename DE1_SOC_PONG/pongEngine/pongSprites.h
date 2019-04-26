#ifndef PONGSPRITES_H
#define PONGSPRITES_H

#include "../pongDisplay/pongDisplay.h"
#include "pongEngine.h"
#include "../BasicFont/BasicFont.h"
#include <string.h>

#define SMALL 0
#define LARGE 1

void pongSprites_initBall( void );
void pongSprites_renderBall(int x, int y, short colour);
void pongSprites_renderPaddle(int x, int y, short colour);
void pongSprites_writeText(int x, int y, int size, char *text, short colour);
void pongSprites_renderRectangle(int x1,int x2, int y1, int y2, int colour);
void pongSprites_changePaddleSize(unsigned int size);
unsigned int pongSprites_getPaddleSizeY( void );

#endif
