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
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_usleep/HPS_usleep.h"
#include <stdbool.h>

#define _BLACK   (0x0000)
#define _WHITE   (0xFFFF)
#define _RED     (0x1F << 11)
#define _GREEN   (0x1F << 6)
#define _BLUE    (0x1F << 0)
#define _YELLOW  (LT24_RED | LT24_GREEN)
#define _CYAN    (LT24_GREEN | LT24_BLUE)
#define _MAGENTA (LT24_BLUE | LT24_RED)

void testScreen( void );

void startMenu( void );

#endif /* PONGSCREENS_PONGSCREENS_H_ */
