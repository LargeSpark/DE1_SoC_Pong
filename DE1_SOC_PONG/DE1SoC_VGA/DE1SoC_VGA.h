#ifndef DE1SOC_VGA_H
#define DE1SOC_VGA_H

void VGA_init(unsigned int vgaPixelBuffer_address, unsigned int vgaCharBuffer_address);
void VGA_drawPixel(int x, int y, short colour);
void VGA_clearScreen();

#endif
