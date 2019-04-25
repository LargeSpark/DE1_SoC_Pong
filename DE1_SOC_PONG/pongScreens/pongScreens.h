/*
 * pongScreens.h
 *
 *  Created on: 20 Apr 2019
 *      Author: Sam
 */

#ifndef PONGSCREENS_PONGSCREENS_H_
#define PONGSCREENS_PONGSCREENS_H_

#include "../pongInputs/pongInputs.h"
#include "../pongSound/pongSound.h"
#include "../pongEngine/pongSprites.h"
#include "../pongDisplay/pongDisplay.h"
#include "../DE1SoC_SevenSeg/sevenSeg.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_usleep/HPS_usleep.h"
#include <stdio.h>
#include <stdbool.h>

#define _BLACK   (0x0000)
#define _WHITE   (0xFFFF)
#define _RED     (0xF800)
#define _GREEN   (0x07C0)
#define _BLUE    (0x001F)
#define _YELLOW  (_RED | _GREEN)
#define _CYAN    (_GREEN | _BLUE)
#define _MAGENTA (_BLUE | _RED)

void setMenu(unsigned int _menuSelector, unsigned int _setting);

void testScreen( void );

void testScreeen_AI( void );

void startMenu( void );

#endif /* PONGSCREENS_PONGSCREENS_H_ */
