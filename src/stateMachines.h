/*
 * Display.h
 *
 *  Created on: 13 Nov 2019
 *      Author: s1808875
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "gpio_init.h"

void updateDisplay();
void refreshDisplay();
void initDisplay();
void outputToDisplay(u16,u16,u16,u8);

extern int PDCount;
extern int BTNPressed;
extern u16 TR1Leds;
extern u16 TR2Leds;

void PD();
void TR1();
void TR2();
void Master();

#endif /* DISPLAY_H_ */
