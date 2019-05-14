/*
 * pongPhysics.c
 *
 *  Created on: 24 Apr 2019
 *      Author: John
 */

#include "pongPhysics.h"

int angle;
int speed;
int rounds; //Number of rounds elapsed
int nrand = 0;
int psrand[] = {293, 326, 46, 329, 228, 35, 100, 197, 345,
		347, 57, 349, 345, 175, 288, 51, 152, 330, 285, 345,
		236, 13, 306, 336, 244, 273, 268, 141, 236, 62, 254,
		11, 100, 17, 35, 296, 250, 114, 342, 12, 158, 137, 276,
		286, 67, 176, 160, 233, 255, 272, 245, 236, 59, 43,
		179, 346, 123, 211, 270, 182, 252, 321, 345, 197,
		50, 54, 303, 293, 335, 126, 71, 222, 170, 127, 299,
		211, 198, 330, 103, 273, 271, 137, 204, 27, 19, 191,
		281, 336, 47, 205, 169, 4, 121, 58, 286, 112, 190,
		60, 217, 235, 248};

//
// Code to regulate serving rules
//

int pongPhysics_serve (void) {
	int angle = psrand[nrand % 99] - 180;
	nrand++;
	return angle;
}

//
// Code to regulate horizontal border collisions
//

int pongPhysics_borderCollision (int speed, int angle) {

int outangle;

outangle	= -angle;

pongEngine_moveBall(outangle, speed);		//Used to avoid ball getting stuck at the border
pongEngine_moveBall(outangle, speed);
pongEngine_moveBall(outangle, speed);

return outangle;

}

// Code to regulate paddle collisions

int* pongPhysics_paddleCollision (int speed, int angle, int player) {

int outangle = 0;
int outspeed = 0;
int deltaAngle;
float sector1 = (0.167 * pongSprites_getPaddleSizeY());
float sector2 = (0.417 * pongSprites_getPaddleSizeY());
float sector3 = (0.083 * pongSprites_getPaddleSizeY());

int output[2];

outangle	=  180 - angle;

// Paddle sector 1 collision / no change in angle or speed
if ((pongEngine_getBallLocation_y() < pongEngine_getPaddleY(player) + (int)(sector1)) && (pongEngine_getBallLocation_y() > pongEngine_getPaddleY(player) - (int)(sector1))) {

	deltaAngle = 0;
	outspeed   = 4;

//Paddle sector 2 collision
} else if ((pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (player) + (int)(sector1)) && (pongEngine_getBallLocation_y() <= pongEngine_getPaddleY (player) + (int)(sector2))) {

	deltaAngle = 10;
	outspeed   = 6;

} else if ((pongEngine_getBallLocation_y() < pongEngine_getPaddleY (player) - (int)(sector1)) && (pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (player) - (int)(sector2))) {

	deltaAngle = -10;
	outspeed	= 6;

//Paddle sector 3 collision
} else if (pongEngine_getBallLocation_y() > pongEngine_getPaddleY(player) + (int)(sector2) && pongEngine_getBallLocation_y() <= pongEngine_getPaddleY(player) + (int)(sector3)) {

	deltaAngle = 20;
	outspeed   = 8;

} else if (pongEngine_getBallLocation_y() < pongEngine_getPaddleY(player) - (int)(sector2) && pongEngine_getBallLocation_y() >= pongEngine_getPaddleY(player) - (int)(sector3)) {

	deltaAngle = -20;
	outspeed   = 8;

} else {

	outspeed = speed;

	if ((pongEngine_getBallLocation_x() <= 50 + 15) && (pongEngine_getBallLocation_x() >= 50 + 10)) {

	outangle = 135;

	} else if ((pongEngine_getBallLocation_x() >= 270 - 15) && (pongEngine_getBallLocation_x() <= 270 - 10)) {

	outangle = 45;

	}
}

outangle = outangle + deltaAngle;


if (outangle > 180) { //outangle < 360
	outangle = -(360 - outangle);
} else if (outangle < -180) {
	outangle = 360 + outangle;
} else if (outangle == 360) { // == 0 = 180 / ==180 = 0
	outangle = 0;
}

output[0] = outangle;
output[1] = outspeed;



pongEngine_moveBall(output[0], output[1]);
pongEngine_moveBall(output[0], output[1]);
pongEngine_moveBall(output[0], output[1]);


return output;


}
