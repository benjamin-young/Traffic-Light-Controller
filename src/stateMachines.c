/*
 * Display.c
 *
 *  Created on: 13 Nov 2019
 *      Author: s1808875
 */
#include <stdio.h>
#include "platform.h"
#include "gpio_init.h"
#include "interface.h"
#include "seg7_display.h"
#include "stateMachines.h"

	/* This file contains functions which manage all of the state machines.
	 *
	 * initDisplay: .
	 * updateDisplay: called at the hardware timer interrupt, manages flow of state machines.
	 * TR1: State machine for traffic light 1.
	 * TR2: State machine for traffic light 2.
	 * PD: State machine for pedestrian crossing.
	 * Master: master state machine for managing other state machines.
	 */

u16 region = 0;
u16 slideSwitchIn = 0;
u16 region_selector;

int PDCount = 0;

//initial sate of state machines
int TR1State =1;
int TR2State =1;
int PDState = 1;
int MasterState =1;

int BTNPressed = 0;
u16 TR1Leds = 0;
u16 TR2Leds = 0;

//12 bit codes for colours
u16 RED = 0b111100000000;
u16 GREEN = 0b000011110000;
u16 YELLOW = 0b111111110000;
u16 WHITE = 0b111111111111;
u16 BLACK = 0b000000000000;

int currentState = 0;
int count = 0;

void initDisplay(){
	outputToDisplay(RED, WHITE, WHITE, 0);
	outputToDisplay(WHITE, RED, WHITE, 1);
	outputToDisplay(RED, WHITE, WHITE, 2);
}

void updateDisplay(){
	//called every 3 seconds
	if(count%750==0){
		xil_printf("%d\n", count);
		Master();
		TR1();
		TR2();
	}
	//called every 0.2 seconds
	if(count%50 == 0){
		//pedestrian light code
		PD();
	}
	count++;
}

// state machine for traffic light 1
// each state calls outputToDisplay function to display the correct traffic light colours
// sets the LED code so that the traffic light is also shown on the board LEDs
// current state is then updated, in state 5 this is conditional on the master state
// or status of traffic detectors

void TR1(){
	switch(TR1State){
		case 1:
			outputToDisplay(RED, WHITE, WHITE, 0);
			TR1Leds = 0b1000000000000000;
			TR1State = 2;

			break;
		case 2:
			outputToDisplay(RED, YELLOW, WHITE, 0);
			TR1Leds = 0b1100000000000000;
			TR1State = 3;

			break;
		case 3:
			outputToDisplay(WHITE, WHITE, GREEN, 0);
			TR1Leds = 0b0010000000000000;
			if((trafficAt2 == 1 || (trafficAt1 == 0 && trafficAt2 == 0))|| BTNPressed == 1){
				TR1State = 4;
			}

			break;
		case 4:
			outputToDisplay(WHITE, YELLOW, WHITE, 0);
			TR1Leds = 0b0100000000000000;
			TR1State = 5;

			break;
		case 5:
			outputToDisplay(RED, WHITE, WHITE, 0);
			TR1Leds = 0b1000000000000000;

			if(MasterState==1){
				TR1State = 2;
			}

		default:
			break;
	}
}


// state machine for traffic light 2
// each state calls outputToDisplay function to display the correct traffic light colours
// sets the LED code so that the traffic light is also shown on the board LEDs
// current state is then updated, in state 1 this is conditional on the master state
// or where there is traffic
void TR2(){
	switch(TR2State){
		case 1:
			outputToDisplay(RED, WHITE, WHITE, 2);
			TR2Leds = 0b0000100000000000;

			if(MasterState==2){
				TR2State = 2;
			}

			break;
		case 2:
			outputToDisplay(RED, YELLOW, WHITE, 2);
			TR2Leds = 0b0000110000000000;

			TR2State = 3;

			break;
		case 3:
			outputToDisplay(WHITE, WHITE, GREEN, 2);
			TR2Leds = 0b0000001000000000;
			if((trafficAt1 == 1 || (trafficAt1 == 0 && trafficAt2 == 0))|| BTNPressed == 1){
				TR2State = 4;
			}

			break;
		case 4:
			outputToDisplay(WHITE, YELLOW, WHITE, 2);
			TR2Leds = 0b0000010000000000;

			TR2State = 5;

			break;
		case 5:
			outputToDisplay(RED, WHITE, WHITE, 2);
			TR2Leds = 0b0000100000000000;

			TR2State = 1;

		default:
			break;
	}
}

// state machine for pedestrian light.
// each state calls outputToDisplay function to display the correct pedestrian light colours.
// current state is then updated.
void PD(){
	switch(PDState){
		case 1:
			outputToDisplay(WHITE,RED,WHITE,1);
			if(MasterState==3){
				PDState = 2;
			}
			break;
		case 2:
			//starts count so that it runs for the correct amount of time
			PDCount++;
			//resests variable now that pedestrians can cross
			BTNPressed=0;
			outputToDisplay(WHITE,GREEN,WHITE,1);
			//when count is 20 has been 4 seconds
			//alternates between states 3 and 2 to flash
			if(PDCount>20){
				PDState = 3;
			}
			break;
		case 3:
			PDCount++;
			outputToDisplay(WHITE,WHITE,WHITE,1);
			//when count has been going on for less that 6 seconds keep flashing
			if(PDCount<=30){
				PDState = 2;
			}
			//return to state 1
			else{
				PDState = 1;
				PDCount = 0;
			}
			break;

		default:
			break;
	}
}

// state machine for Master - controls the other states.

void Master(){
	switch(MasterState){
		//represents TR1
		case 1:
			//TR1 finished and traffic at TR2 or neither
			if(TR1State == 5 && (trafficAt2 || (trafficAt1 == 0 && trafficAt2 == 0))){
				MasterState=2;
			}
			//if pedestrian button is pressed and traffic is only at TR1
			else if(BTNPressed == 1){
				if((trafficAt1 == 1 && trafficAt2 == 0)){
					//TR1 finished
					if(TR1State == 5){
						MasterState=3;
					}
				}

			}
			break;
		//represents TR2
		case 2:
			if(BTNPressed==1 && TR2State == 5){
				MasterState=3;
			}
			else if (BTNPressed==0 && TR2State == 5 && (trafficAt1 ==1 || (trafficAt1 ==0 && trafficAt2 ==0))){
				MasterState=1;
			}
			break;
		//represents PD
		case 3:
			//if PD is finished
			if(PDState == 3){
				//traffic is only at TR2
				if(trafficAt1 == 0 && trafficAt2 == 1){
					MasterState = 2;
				}
				//traffic at TR1 or neither
				if(trafficAt1 == 1 || (trafficAt1 ==0 && trafficAt2==0)){
					MasterState = 1;
				}

			}
			break;

		default:
			break;
	}
}





