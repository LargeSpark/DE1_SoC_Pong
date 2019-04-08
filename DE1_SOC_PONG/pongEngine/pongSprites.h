#ifndef PONGDISPLAY_H
#define PONGDISPLAY_H

#include "../pongDisplay/pongDisplay.h"
#include "../BasicFont/BasicFont.h"
#include <string.h>

#define SMALL 0
#define LARGE 1

void pongSprites_init();
void pongSprites_initBall();
void pongSprites_renderBall(int x, int y, short colour);
void pongSprites_renderPaddle(int x, int y, short colour);
void pongSprites_writeText(int x, int y, int size, char *text, short colour);
void pongSprites_renderRectangle(int x1,int x2, int y1, int y2, short colour);

#endif
