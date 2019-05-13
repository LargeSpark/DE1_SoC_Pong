/*
 * pongPhysics.h
 *
 *  Created on: 24 Apr 2019
 *      Author: John
 */

#ifndef PONGENGINE_PONGPHYSICS_H_
#define PONGENGINE_PONGPHYSICS_H_

#include <cmath>
#include <stdlib.h>
#include "pongSprites.h"
#include "pongEngine.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../pongDisplay/pongDisplay.h"


int pongPhysics_serve (void);
int pongPhysics_borderCollision (int speed, int angle);
int* pongPhysics_paddleCollision (int speed, int angle, int player);






#endif /* PONGENGINE_PONGPHYSICS_H_ */
