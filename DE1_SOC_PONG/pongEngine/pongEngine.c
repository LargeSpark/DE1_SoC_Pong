#include "pongEngine.h"
//320x240
/*############ Ball Globals ############*/
//Calculated ball path
struct ballPathInst{
	int x;
	int y;
};
int ballPathInstCounter = 0;
int ballCurrentPosPath = 0;
struct ballPathInst BallPath[1000];

int ballAngle = 361; //Set to impossible angle to ensure on initialisation there
short ballColour = 0xFFFF; //Ball colour
int ballX = centre_x; //ball defaults
int ballY = centre_y; //ball defaults

/*############ Paddle Globals ############*/
//Paddle thresholds
int paddleMinY = 30;
int paddleMaxY = 210;
//Paddle Locations
int paddle1Y = centre_y;
int paddle1X = 50;
int paddle2Y = centre_y;
int paddle2X = 270;
//Paddle Colours
int paddle1Colour = 0xFFFF;
int paddle2Colour = 0xFFFF;

/*############ Ball Functions ############*/
void pongEngine_moveBall(int angle, int speed){
	ResetWDT();
	//Add speed here
	//If angle change then calculate new instructions
	if(angle == ballAngle){

		//To prevent overflow
		if(ballCurrentPosPath > ballPathInstCounter){
			ballCurrentPosPath = ballPathInstCounter;
		}
		//render ball and move position
		ballX = BallPath[ballCurrentPosPath].x;
		ballY = BallPath[ballCurrentPosPath].y;
		pongSprites_renderBall(ballX, ballY, ballColour);
		ResetWDT();
		ballCurrentPosPath++;
	}
	else{
		ballAngle = angle;
		ResetWDT();
		pongEngine_calcBallPathInst(angle);
		ballX = BallPath[ballCurrentPosPath].x;
		ballY = BallPath[ballCurrentPosPath].y;
		pongSprites_renderBall(ballX, ballY, ballColour);
		ballCurrentPosPath++;
	}
}

void pongEngine_setBallLocation(int x, int y){
	pongEngine_destroyBall();
	ResetWDT();
	ballX = x;
	ballY = y;
	pongEngine_calcBallPathInst(ballAngle);
	pongSprites_renderBall(ballX, ballY, ballColour);
	ballCurrentPosPath++;
}

int pongEngine_getBallAngle(){
	return ballAngle;
}

int pongEngine_getBallLocation_x(){
	return ballX;
}

int pongEngine_getBallLocation_y(){
	return ballY;
}

void pongEngine_destroyBall(){
	pongSprites_renderBall(ballX, ballY, 0x0000);
}

void pongEngine_calcBallPathInst(int angle){
	int x;
	int newangle;
	int foundAngle = 0;
	int angleX;
	int angleY;
	if(foundAngle == 0){
		for(x = 0; x<240; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,0,x);
			if(newangle == angle){
				foundAngle = 1;
				angleX = 0;
				angleY = x;
			}
		}
	}
	if(foundAngle == 0){
		for(x = 0; x<240; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,320,x);
			if(newangle == angle){
				foundAngle = 1;
				angleX = 0;
				angleY = x;
			}
		}
	}
	if(foundAngle == 0){
		for(x = 0; x<320; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,x,0);
			if(newangle == angle){
				foundAngle = 1;
				angleX = x;
				angleY = 0;
			}
		}
	}
	if(foundAngle == 0){
		for(x = 0; x<320; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,x,240);
			if(newangle == angle){
				foundAngle = 1;
				angleX = x;
				angleY = 0;
			}
		}
	}
	pongEngine_genBallPathInst(ballX, ballY, angleX, angleY);
}

//Line drawing code from Assignement 1 - Alexander Bolton
void pongEngine_genBallPathInst(int x1, int y1, int x2, int y2){
	//REFERENCE: drawLine using Bresenhams algorithm. https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm
	//reset ballPathCounts

	//calculate deltas
	int dx =  abs (x2 - x1);
	int dy = -abs (y2 - y1);
	//calculate error
	int error = dx + dy;
	int error2;
	//Find quadrant
	int sy;
	int sx;
	ballPathInstCounter = 0;
	ballCurrentPosPath = 0;
	if(x1<x2){
		sx = 1;
	}
	else{
		sx = -1;
	}

	if(y1<y2){
		sy = 1;
	}
	else{
		sy = -1;
	}
	//Loop though and calculate line pixels
	while(1){
		//Generate Instructions
		BallPath[ballPathInstCounter].x = x1;
		BallPath[ballPathInstCounter].y = y1;
		ballPathInstCounter++;
		if (x1 == x2 && y1 == y2){ break;}
		error2 = 2 * error;
		//if error2 is larger than delta y then add 1 to x
		if (error2 >= dy) {
			error += dy;
			x1 += sx;
		}
		//if error2 is smaller than delta x then add 1 to y
		if (error2 <= dx) {
			error += dx;
			y1 += sy;
		}
  }
}

int pongEngine_calcAngle(int x1, int y1, int x2, int y2){
	double diff_y = y1 - y2;
	double diff_x = x1 - x2;
	double angle = atan2(diff_y, diff_x);
	float ang_d = angle * 180/PI;
	int ang_dint = ang_d;
	return ang_dint;
}
/*############ Paddle Functions ############*/
void pongEngine_paddleSetYLimits(int maxy, int miny){
	paddleMinY = miny;
	paddleMaxY = maxy;
}

void pongEngine_paddleSetYLocation(int player, int y){
	if(y>paddleMaxY){
		y = paddleMaxY;
	}
	if(player == 1){
		paddle1Y = y;
		pongSprites_renderPaddle(paddle1X, y, paddle1Colour);
	}
	if(player == 2){
		paddle2Y = y;
		pongSprites_renderPaddle(paddle2X, y, paddle2Colour);
	}
}

void pongEngine_paddleSetXLocation(int player, int x){
	if(player == 1){
		paddle1X = x;
		pongSprites_renderPaddle(x, paddle1Y, paddle1Colour);
	}
	if(player == 2){
		paddle2X = x;
		pongSprites_renderPaddle(x, paddle1Y, paddle2Colour);
	}
}

void pongEngine_paddleMove(int player, int direction, int speed){
	/*
	UP - 1
	DOWN - 0
	*/
	if(player == 1){
		if(direction == UP){
			pongEngine_paddleSetYLocation(1, paddle1Y+speed);
		}
		if(direction == DOWN){
			pongEngine_paddleSetYLocation(1, paddle1Y-speed);
		}
	}
	if(player == 2){
		if(direction == UP){
			pongEngine_paddleSetYLocation(1, paddle2Y+speed);
		}
		if(direction == DOWN){
			pongEngine_paddleSetYLocation(1, paddle2Y-speed);
		}
	}
}

void pongEngine_paddleDestory(int player){
	if(player == 1){
		pongSprites_renderPaddle(paddle1X, paddle1Y, 0x0000);
	}
	if(player == 2){
		pongSprites_renderPaddle(paddle2X, paddle2Y, 0x0000);
	}
}
