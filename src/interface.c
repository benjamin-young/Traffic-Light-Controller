/*
 * Interface.c
 *
 *  Created on: 15 Nov 2019
 *      Author: s1808875
 */
#include <stdio.h>
#include "platform.h"
#include "gpio_init.h"
#include "stateMachines.h"
#include "seg7_display.h"
#include "interface.h"

	/* This file contains functions which manage inputs and outputs.
	 * outputToDisplay: outputs to the 4x3 grid.
	 * PDCountDisplay: calls the displayNumber function in seg7_display.c.
	 * 				   Either displays the countdown for the pedestrian
	 * 				   or simply displays '0'.
	 * buttonCheck: checks if up button is being pressed and stores in a variable.
	 * LEDOutput: uses the bitwise OR operation on the LED codes to combine them
	 * 			  and outputs the result to the board LEDs.
	 * switchCheck:
	 * async: function is called in the main function. Contains an infinite while loop
	 *        and calls the other functions that need to be continuously called.
	 */

u16 pushBtnUpIn = 0;
u16 SegOutput = 1;

u16 BTNLeds;


void outputToDisplay(u16 row1, u16 row2, u16 row3, u8 column){
	switch(column){
	case 0:
		XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, row1);
		XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, row2);
		XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, row3);
		break;
	case 1:
		XGpio_DiscreteWrite(&REGION_3_COLOUR, 1, row1);
		XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, row2);
		XGpio_DiscreteWrite(&REGION_5_COLOUR, 1, row3);
		break;
	case 2:
		XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, row1);
		XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, row2);
		XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, row3);
		break;
	case 3:
		XGpio_DiscreteWrite(&REGION_9_COLOUR, 1, row1);
		XGpio_DiscreteWrite(&REGION_10_COLOUR, 1, row2);
		XGpio_DiscreteWrite(&REGION_11_COLOUR, 1, row3);
		break;
	default:
		XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, 0);
		XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, 0);
		XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, 0);
		break;
	}
}

void PDCountDisplay(){
	if(PDCount ==0){
		displayNumber(0);
	}
	else{
		//converts counter that counts to 30 in 6 seconds to 6 second count down
		//sends that number to be displayed on 7 segment display
		displayNumber((30-PDCount)/5 +1);
	}
}

void buttonCheck()
{
	pushBtnUpIn = XGpio_DiscreteRead(&P_BTN_UP, 1);
	if (pushBtnUpIn == 1) {
		//variable stores whether the up button has been pressed
		BTNPressed = 1;
	}
	if(BTNPressed == 1){
		//16 bit LED code for pedestrian waiting
		BTNLeds = 0b0000000000000001;
	}
	else{
		BTNLeds = 0b0000000000000000;
	}
}

void switchCheck(){
	switches = XGpio_DiscreteRead(&SLIDE_SWITCHES,1);
	//isolates the 2 LSBs
	switches = switches<<14 >>14;
	switch(switches){
	case 0:
		//traffic at neither
		trafficAt1 = 0;
		trafficAt2 = 0;
		break;
	case 1:
		//traffic at 1
		trafficAt1 = 1;
		trafficAt2 = 0;
		break;
	case 2:
		//traffic at 2
		trafficAt1 = 0;
		trafficAt2 = 1;
		break;
	case 3:
		//traffic at 1 and 2
		trafficAt1 = 1;
		trafficAt2 = 1;
		break;
	}
}

void LEDOutput(){
	//Bitwise OR operation effectively combines the LED codes
	u16 output = TR1Leds|TR2Leds|BTNLeds;
	XGpio_DiscreteWrite(&LED_OUT, 1, output);
}

void async(){
	while(1){
		buttonCheck();
		PDCountDisplay();
		LEDOutput();
		switchCheck();
	}
}
