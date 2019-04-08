#ifndef PONGENGINE_H
#define PONGENGINE_H

#include "pongSprites.h"
#include <cmath>
#include "../HPS_Watchdog/HPS_Watchdog.h"

#define PI 3.14159265359
#define centre_x 160
#define centre_y 120
#define UP 1
#define DOWN 0

//Publicly used
/*############ Ball Functions ############*/
void pongEngine_moveBall(int angle, int speed);
void pongEngine_destroyBall();
void pongEngine_setBallLocation(int x, int y);
int pongEngine_getBallAngle();
int pongEngine_getBallLocation_x();
int pongEngine_getBallLocation_y();
/*############ Paddle Functions ############*/
pongEngine_paddleMove(int player, int direction, int speed);
void pongEngine_paddleSetYLimits(int maxy, int miny);
void pongEngine_paddleSetYLocation(int player, int y);
void pongEngine_paddleSetXLocation(int player, int x);

//used within library
/*############ Ball Functions ############*/
int pongEngine_calcAngle(int x1, int y1, int x2, int y2);
void pongEngine_calcBallPathInst(int angle);
void pongEngine_genBallPathInst(int x1, int y1, int x2, int y2);
/*############ Paddle Functions ############*/

#endif
