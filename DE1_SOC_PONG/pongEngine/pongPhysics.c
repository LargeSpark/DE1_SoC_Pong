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

//
// Code to regulate serving rules
//

int pongPhysics_serve (void) {

	int r = rand() % 1;	//Choose random number between 0 and 9
	int serveSpeed = 2 ;
	int pr = 0;					//Previous receiver (Player 1 or player 2)

	if (rounds < 1) {			//First serve
		if (r <= 4) {			//Player 1 receives
			angle = 0;
			pr	  = 1;
			rounds++;
		} else if (r > 5) {		//Player 2 receives
			angle = 180;
			pr	  = 2;
			rounds++;
		}
	} else {					//Alternate thereafter
		if (pr == 1) {
			angle = 180;
			pr	  = 2  ;
			rounds++;
		} else if (pr == 2) {
			angle = 0  ;
			pr 	  = 1  ;
			rounds++;
		}

	}

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

	deltaAngle = -10;
	outspeed   = 6;

} else if ((pongEngine_getBallLocation_y() < pongEngine_getPaddleY (player) - (int)(sector1)) && (pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (player) - (int)(sector2))) {

	deltaAngle = 10;
	outspeed	= 6;

//Paddle sector 3 collision
} else if (pongEngine_getBallLocation_y() > pongEngine_getPaddleY(player) + (int)(sector2) && pongEngine_getBallLocation_y() <= pongEngine_getPaddleY(player) + (int)(sector3)) {

	deltaAngle = -20;
	outspeed   = 8;

} else if (pongEngine_getBallLocation_y() < pongEngine_getPaddleY(player) - (int)(sector2) && pongEngine_getBallLocation_y() >= pongEngine_getPaddleY(player) - (int)(sector3)) {

	deltaAngle = 20;
	outspeed   = 8;

} else {

	outspeed = speed;

	if ((pongEngine_getBallLocation_x() <= 50 + 15) && (pongEngine_getBallLocation_x() >= 50 + 10)) {

	outangle = 135;

	} else if ((pongEngine_getBallLocation_x() >= 270 - 15) && (pongEngine_getBallLocation_x() <= 270 - 10)) {

	outangle = 45;

	}
}



/*
if (outangle > 180) { //outangle < 360
	outangle = -(360 - outangle);
} else if (outangle < -180) {
	outangle = 360 + outangle;
} else if (outangle == 360) { // == 0 = 180 / ==180 = 0
	outangle = 0;
}

*/

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
