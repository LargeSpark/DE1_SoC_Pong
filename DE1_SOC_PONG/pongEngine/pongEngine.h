#ifndef PONGENGINE_H
#define PONGENGINE_H

#include "pongSprites.h"
#include <cmath>
#include "../HPS_Watchdog/HPS_Watchdog.h"

#define PI 3.14159265359
#define centre_x 160
#define centre_y 120

//Publicly used
void pongEngine_moveBall(int angle, int speed);
void pongEngine_destroyBall();
void pongEngine_setBallLocation(int x, int y);
int pongEngine_getBallAngle();
int pongEngine_getBallLocation_x();
int pongEngine_getBallLocation_Y();

//used within library
int pongEngine_calcAngle(int x1, int y1, int x2, int y2);
void pongEngine_calcBallPathInst(int current_x, int current_y, int angle);
void pongEngine_genBallPathInst(int x1, int y1, int x2, int y2);

#endif
