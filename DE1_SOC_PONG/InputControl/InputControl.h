/*
 * InputControl.h
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

#ifndef INPUTCONTROL_INPUTCONTROL_H_
#define INPUTCONTROL_INPUTCONTROL_H_

#include "../pongEngine/pongEngine.h"

// Definitions for modes
#define MENUS 0
#define GAME  1

// Set paddle speed
#define keySpeed  20
#define keyBSpeed 10

// Takes input from keyboard/keys and acts on it depending on screen mode
void Input(unsigned int key, unsigned int speed);

// Set screen mode. Add switching between menus?
void InputMode(unsigned int _mode);

#endif /* INPUTCONTROL_INPUTCONTROL_H_ */
