/*
 * interface.h
 *
 *  Created on: 15 Nov 2019
 *      Author: s1808875
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

void outputToDisplay(u16, u16, u16, u8);
void buttonCheck();
void PDCountDisplay();
void async();
void switchCheck();

u16 switches;

int trafficAt1;
int trafficAt2;


#endif /* INTERFACE_H_ */
