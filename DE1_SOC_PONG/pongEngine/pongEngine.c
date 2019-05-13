#include "pongEngine.h"

//320x240

/*############ Ball Globals ############*/
//Calculated ball path
struct ballPathInst{
	int x;
	int y;
};
//instruction counter
int ballPathInstCounter = 0;
//current instruction
int ballCurrentPosPath = 0;
//array of structs
struct ballPathInst BallPath[1000];

int ballAngle = 361; //Set to impossible angle to ensure on initialisation there
short ballColour = 0xFFFF; //Ball colour
int ballX = centre_x; //ball defaults
int ballY = centre_y; //ball defaults
//Thresholds
int ballMinX = 8;
int ballMaxX = 312;
int ballMinY = 23;
int ballMaxY = 232;

/*############ Paddle Globals ############*/
//Paddle thresholds
int paddleMinY = 50;
int paddleMaxY = 210;
//Paddle Locations
int paddle1Y = centre_y;
int paddle1X = 50;
int paddle2Y = centre_y;
int paddle2X = 270;
//Paddle Colours
int paddle1Colour = 0xFFFF;
int paddle2Colour = 0xFFFF;
/*############ Score Keeping Globals############*/
int player1Score = 0;
int player2Score = 0;
/*############ General Functions ############*/
//This function initialises the pong engine
void pongEngine_init(){
	int topAdjust = 15; //VGA Pixel faults from y
	Displays_clearScreen(); //clear screen
	pongEngine_resetPaddles(); //reset paddles
	pongEngine_resetBallLoc(); //reset ball location

	pongEngine_paddleCreate(1); //create paddle
	pongEngine_paddleCreate(2); //create paddle


	//create arena
	//create green area
	pongSprites_renderRectangle(320,0, topAdjust, 0, (0x1F << 11)); //to create red border on LCD
	paddleMinY = pongSprites_getPaddleSizeY()/2;//set limits
	paddleMaxY = 240-paddleMinY; //set limits

	pongEngine_paddleSetYLimits(paddleMaxY, paddleMinY+topAdjust+1); //adjust limits
}
/*############ Score Keeping ############*/
//This function adds points to a player
void pongEngine_addPoint(int player){
	//check player and add point to player
	if(player == 1){
		player1Score++;
	}
	if(player == 2){
		player2Score++;
	}
	//refresh score
	pongEngine_refreshScore();
}
//This function resets the score to zero
void pongEngine_resetScore(unsigned int _Refresh){
	player1Score = 0;
	player2Score = 0;
	if (_Refresh != 0){
		pongEngine_refreshScore(); //refresh score
	}
}

//refresh score to board seven seg and on screen
void pongEngine_refreshScore(){
	//create char
	char playerscorech[6];
	//split numbers
	int P1number0, P1number1, P2number0, P2number1;

	// Fix overflow past 100 - loops to zero
	player1Score = player1Score %100;
	player2Score = player2Score %100;
	//split numbers into individual characters
	P1number0 = player1Score/10; //1
	P1number1 = player1Score % 10; //5
	P2number0 = player2Score/10; //1
	P2number1 = player2Score % 10; //5
	//updates 7 seg
	SDisplay_PNum(player1Score,2);
	SDisplay_PNum(player2Score,0);
	//updates score
	playerscorech[0] = P1number0 + '0';
	playerscorech[1] = P1number1 + '0';
	playerscorech[2] = ' ';
	playerscorech[3] = P2number0 + '0';
	playerscorech[4] = P2number1 + '0';
	playerscorech[5] = '\0';
	//clears score by drawing black rectangle on it
	pongSprites_renderRectangle(centre_x - 30,centre_x+ 30 , 17, 17+14, 0x00);
	//writes score on screen
	pongSprites_writeText(centre_x - 28, 17, 1, playerscorech, 0xFFFF);
	//Displays_forceRefresh();
}

/*############ Ball Functions ############*/
//This function moves the pong ball
void pongEngine_moveBall(int angle, int speed){
	ResetWDT(); //reset watchdog
	pongEngine_destroyBall(); //destroy the ball so its black
	//Check if at thresholds
	if(ballX < ballMinX){
		ballX = ballMinX;
		ballCurrentPosPath = ballCurrentPosPath-speed;
	}
	if(ballX > ballMaxX){
		ballX = ballMaxX;
		ballCurrentPosPath = ballCurrentPosPath-speed;
	}
	if(ballY < ballMinY){
		ballY = ballMinY;
		ballCurrentPosPath = ballCurrentPosPath-speed;
	}
	if(ballY > ballMaxY){
		ballY = ballMaxY;
		ballCurrentPosPath = ballCurrentPosPath-speed;
	}
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
		//Move ball speed amount
		ballCurrentPosPath = ballCurrentPosPath+speed;
	}
	//if ball is at a different angle
	else{
		ballAngle = angle; //set ballAngle to angle
		ResetWDT();
		//Calculate new path
		pongEngine_calcBallPathInst(angle);
		//render ball and move position
		ballX = BallPath[ballCurrentPosPath].x;
		ballY = BallPath[ballCurrentPosPath].y;
		pongSprites_renderBall(ballX, ballY, ballColour);
		//Move ball speed amount
		ballCurrentPosPath = ballCurrentPosPath+speed;
	}
}
//reset ball location
void pongEngine_resetBallLoc( void ){
	//pongEngine_setBallLocation(centre_x, centre_y); // Avoiding extra ball on startup
	ballX = centre_x;
	ballY = centre_y;
}
//set ball location
void pongEngine_setBallLocation(int x, int y){
	pongEngine_destroyBall();
	ResetWDT();
	ballX = x;
	ballY = y;
	pongEngine_calcBallPathInst(ballAngle);
	pongSprites_renderBall(ballX, ballY, ballColour);
	ballCurrentPosPath++;
}
//get ball angle
int pongEngine_getBallAngle(void){
	return ballAngle;
}
//get ball location x
int pongEngine_getBallLocation_x(void){
	return ballX;
}
//get ball location y
int pongEngine_getBallLocation_y(void){
	return ballY;
}
//create the ball
void pongEngine_createBall(void){
	pongSprites_renderBall(ballX, ballY, ballColour);
}
//destroy the ball
void pongEngine_destroyBall(void){
	pongSprites_renderBall(ballX, ballY, 0x0000);
}
//calculate the ball path instructions
void pongEngine_calcBallPathInst(int angle){
	int x;
	int newangle;
	int foundAngle = 0;
	int angleX;
	int angleY;
	//Prevent floating point error at right angles.
	if(angle == 0){
		foundAngle = 1;
		angleX = 0;
		angleY = ballY;
	}
	if(angle == 90){
		foundAngle = 1;
		angleX = ballX;
		angleY = 0;
	}
	if(angle == 180){
		foundAngle = 1;
		angleX = 320;
		angleY = ballY;
	}
	if(angle == 270){
		foundAngle = 1;
		angleX = ballX;
		angleY = 240;
	}
	//Finds angle by working out angle between 2 points (one being the centre of ball, one being edge of screen)
	//if angle found do not calculate the angle
	if(foundAngle == 0){
		for(x = 0; x<240; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,0,x); //calculate angle
			if(newangle == angle){
				foundAngle = 1;
				angleX = 0;
				angleY = x;
			}
		}
	}
	if(foundAngle == 0){
		for(x = 0; x<240; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,320,x); //calculate angle
			if(newangle == angle){
				foundAngle = 1;
				angleX = 320;
				angleY = x;
			}
		}
	}
	if(foundAngle == 0){
		for(x = 0; x<320; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,x,0); //calculate angle
			if(newangle == angle){
				foundAngle = 1;
				angleX = x;
				angleY = 0;
			}
		}
	}
	if(foundAngle == 0){
		for(x = 0; x<320; x++){
			newangle = pongEngine_calcAngle(ballX,ballY,x,240); //calculate angle
			if(newangle == angle){
				foundAngle = 1;
				angleX = x;
				angleY = 240;
			}
		}
	}
	//generate ball instructions
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

//This function calculates the angle between 2 points
int pongEngine_calcAngle(int x1, int y1, int x2, int y2){
	//calculate deltas
	double diff_y = y1 - y2;
	double diff_x = x1 - x2;
	//calculate angle
	double angle = atan2(diff_y, diff_x);
	//turn to degrees
	float ang_d = angle * 180/PI;
	//turn to integer
	int ang_dint = ang_d;
	//return
	return ang_dint;
}
/*############ Paddle Functions ############*/
//set paddle limits on y axis
void pongEngine_paddleSetYLimits(int maxy, int miny){
	paddleMinY = miny;
	paddleMaxY = maxy;
}
//set paddle y location
void pongEngine_paddleSetYLocation(int player, int y){
	pongEngine_paddleDestroy(player); //destroy paddle to turn to black
	//check thresholds
	if(y>paddleMaxY){
		y = paddleMaxY;
	}
	if(y<paddleMinY){
		y = paddleMinY;
	}
	//check which player and move appropriate paddle
	if(player == 1){
		paddle1Y = y;
		pongSprites_renderPaddle(paddle1X, y, paddle1Colour); //render paddle
	}
	if(player == 2){
		paddle2Y = y;
		pongSprites_renderPaddle(paddle2X, y, paddle2Colour); //render paddle
	}
}

//set paddle x location
void pongEngine_paddleSetXLocation(int player, int x){
	pongEngine_paddleDestroy(player); //destroy paddle
	//check player and set appropriate paddle
	if(player == 1){
		paddle1X = x;
		pongSprites_renderPaddle(x, paddle1Y, paddle1Colour); //render paddle
	}
	if(player == 2){
		paddle2X = x;
		pongSprites_renderPaddle(x, paddle1Y, paddle2Colour); //render paddle
	}
}
//This function moves the paddle
void pongEngine_paddleMove(int player, int direction, int speed){
	/*
	UP - 1
	DOWN - 0
	*/
	int moveto;
	//check player
	if(player == 1){
		//check direction
		if(direction == UP){
			moveto = paddle1Y+speed; //calculate y to move to
			pongEngine_paddleSetYLocation(1, moveto); //render
		}
		if(direction == DOWN){
			moveto = paddle1Y-speed; //calculate y to move to
			pongEngine_paddleSetYLocation(1, moveto); //render
		}
	}
	//check player
	if(player == 2){
		//check direction
		if(direction == UP){
			moveto = paddle2Y+speed; //calculate y to move to
			pongEngine_paddleSetYLocation(2, moveto); //render
		}
		if(direction == DOWN){
			moveto = paddle2Y-speed; //calculate y to move to
			pongEngine_paddleSetYLocation(2, moveto); //render
		}
	}
}
//This function creates the paddle
void pongEngine_paddleCreate(int player){
	//check player
	if(player == 1){
		pongSprites_renderPaddle(paddle1X, paddle1Y, paddle1Colour); //render paddle
	}
	if(player == 2){
		pongSprites_renderPaddle(paddle2X, paddle2Y, paddle2Colour); //render paddle
	}
}
//This function destroys the paddle
void pongEngine_paddleDestroy(int player){
	//check player
	if(player == 1){
		pongSprites_renderPaddle(paddle1X, paddle1Y, 0x0000); //render paddle black
	}
	if(player == 2){
		pongSprites_renderPaddle(paddle2X, paddle2Y, 0x0000); //render paddle black
	}
}
//This function resets the paddles
void pongEngine_resetPaddles(){
	pongEngine_paddleDestroy(1); //destroy paddles
	pongEngine_paddleDestroy(2);
	pongEngine_paddleSetYLocation(1, centre_y); //set paddle location
	pongEngine_paddleSetYLocation(2, centre_y);
}
//This function gets the paddle locations
unsigned int pongEngine_getPaddleY(unsigned int paddle){
	if (paddle == 1){
		return paddle1Y;
	} else {
		return paddle2Y;
	}
}
