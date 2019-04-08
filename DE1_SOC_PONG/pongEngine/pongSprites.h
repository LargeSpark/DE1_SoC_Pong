#ifndef PONGDISPLAY_H
#define PONGDISPLAY_H

#include "../pongDisplay/pongDisplay.h"
void pongSprites_init();
void pongSprites_initBall();
void pongSprites_renderBall(int x, int y, short colour);
void pongSprites_renderPaddle(int x, int y, short colour);
#endif
