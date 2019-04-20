#ifndef PS2_KEYBOARD_PS2_KEYBOARD_H_
#define PS2_KEYBOARD_PS2_KEYBOARD_H_

#include <stdbool.h>
#include "../pongEngine/pongEngine.h"
#include "../pongDisplay/pongDisplay.h"
#include <string.h>
#include "../HPS_IRQ/HPS_IRQ.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include <stdio.h>
#include <stdlib.h>


// Keyboard scancodes for scan set 2
// From https://techdocs.altium.com/display/FPGA/PS2+Keyboard+Scan+Codes
#define _ESC 	0x76
#define _RETURN	0x5A
#define _W		0x1D
#define _A		0x1C
#define _S		0x1B
#define _D		0x23
#define _LEFT	0x6B
#define _RIGHT	0x74
#define _UP		0x75
#define _DOWN	0x72

// These are all pretty much used exclusively by this library. Interrupts handle everything and pass to InputControl.
void keyboardInitialise(void);
char getKey(void);
void inputKeyboard( void );
char PS2Scan(void);
signed int paddleDir(int paddle);
void emptyFIFO( void );

#endif /* PS2_KEYBOARD_PS2_KEYBOARD_H_ */
