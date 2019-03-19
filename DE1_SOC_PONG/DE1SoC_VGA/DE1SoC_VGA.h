#ifndef DE1SOC_VGA_H
#define DE1SOC_VGA_H

void VGA_init(unsigned volatile int VGA);
void VGA_drawPixel(int x, int y, short colour);
void VGA_clearScreen(int x, int y, short colour);

#endif
