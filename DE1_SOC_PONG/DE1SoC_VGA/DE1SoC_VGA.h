#ifndef DE1SOC_VGA_H
#define DE1SOC_VGA_H

void VGA_init(short vgaPixelBuffer_address, short vgaCharBuffer_address);
void VGA_drawPixel(int x, int y, short colour);
void VGA_drawPixelToHwBuffer(int x, int y, short colour);
void VGA_BufferSwap();
void VGA_clearScreen();

#endif
