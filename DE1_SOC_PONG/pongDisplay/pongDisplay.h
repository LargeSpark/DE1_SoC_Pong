#ifndef PONGDISPLAY_H
#define PONGDISPLAY_H

#include "../DE1Soc_LT24/DE1Soc_LT24.h"
#include "../DE1Soc_VGA/DE1Soc_VGA.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"

#define SOFTWAREFB 1
#define SOFTWAREQUADFB 2
#define HARDWARE 3

void Displays_init(volatile short vga_PixelAddress, unsigned volatile int vga_CharacterAddress, unsigned volatile int lcd_pio_base, unsigned volatile int lcd_hw_base);
void Displays_drawPixel(int x, int y, short colour);
void Displays_clearScreen();
void Displays_Refresh();
void Displays_setPixel(int x, int y, short colour);
void Displays_mode(int mode);
#endif
