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

int outangle;
int outspeed;
int deltaAngle;

int output[2];

// Paddle sector 1 collision / no change in angle or speed
if ((pongEngine_getBallLocation_y() < pongEngine_getPaddleY (player) + 10) && (pongEngine_getBallLocation_y() > pongEngine_getPaddleY (player) - 10)) {

	deltaAngle = 0;
	outspeed   = 2;

//Paddle sector 2 collision
} else if ((pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (player) + 10) && (pongEngine_getBallLocation_y() <= pongEngine_getPaddleY (player) + 25)) {

	deltaAngle = -10;
	outspeed   = 3;

} else if ((pongEngine_getBallLocation_y() < pongEngine_getPaddleY (player) - 10) && (pongEngine_getBallLocation_y() >= pongEngine_getPaddleY (player) - 25)) {

	deltaAngle = 10;
	outspeed	= 3;

//Paddle sector 3 collision
} else if (pongEngine_getBallLocation_y() > pongEngine_getPaddleY(player) + 25 && pongEngine_getBallLocation_y() <= pongEngine_getPaddleY(player) + 30) {

	deltaAngle = -20;
	outspeed   = 4;

} else if (pongEngine_getBallLocation_y() < pongEngine_getPaddleY(player) - 25 && pongEngine_getBallLocation_y() >= pongEngine_getPaddleY(player) - 30) {

	deltaAngle = 20;
	outspeed   = 4;
}
outangle	=  180 - angle;


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
