/*
 * InputControl.c
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */
#include "InputControl.h"

volatile unsigned mode = GAME;

void Input(unsigned int key, unsigned int speed){
	if (mode == GAME){
		if (key == 1){
			pongEngine_paddleMove(1, DOWN, 	speed);
		}
		else if (key == 2){
			pongEngine_paddleMove(1, UP, 	speed);
		}
		else if (key == 3){
			pongEngine_paddleMove(2, DOWN, 	speed);
		}
		else if (key == 4){
			pongEngine_paddleMove(2, UP, 	speed);
		}
	}
}

void InputMode(unsigned int _mode){
	if (_mode == MENUS) { mode = MENUS; } else { mode = GAME; }
}
