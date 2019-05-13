//This library allows the control of the 7 segment display
#include "sevenSeg.h"
//Hex numbers in an array
int HexSDisplay[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

//This function sets the 7 segment displays
void SDisplay_set(int Display, int HexValue){
	//Hex memory base
	volatile int *HEX0 = (int* ) 0XFF200020;
	volatile int *HEX1 = (int* ) 0XFF200030;
	//Values
	int invClearBits = 0x7F; //inverted bits to put through and bitwise
	int shiftAmount = 8; //shift multiple amount
	int hex1Adjust = 4; //adjust amount for second memory address
	//check each number position to work out between addresses
	if(Display < 4){
		*HEX0 &= ~(invClearBits << (Display * shiftAmount)); //clear bits
		*HEX0 |= (HexSDisplay[HexValue] << (Display * shiftAmount)); //set bits
	}
	else{
		*HEX1 &= ~(invClearBits << ((Display - hex1Adjust) * shiftAmount)); //clear bits
		*HEX1 |= (HexSDisplay[HexValue] << ((Display - hex1Adjust) * shiftAmount)); //set bits
	}
}
//This function clears the seven seg displays
void SDisplay_clearAll(){
	//Hex memory base
	volatile int *HEX0 = (int* ) 0XFF200020;
	volatile int *HEX1 = (int* ) 0XFF200030;
	int zero = 0x00;//Zero
	//set displays
	*HEX0 &= zero; //clear bits
	*HEX1 &= zero; //clear bits
}
//This function sets a number per pair of 7 segment displays
void SDisplay_PNum(int number, int pair){
	//Example is 15
	//Split number in 2
	int number0 = number/10; //1
	int number1 = number % 10; //2
	//calculate pair of displays from the pair set in the function call
	int pairdisplay0 = 2*pair;
	int pairdisplay1 = 2*pair+1;
	//set displays
	SDisplay_set(pairdisplay0,number1);
	SDisplay_set(pairdisplay1,number0);
}
