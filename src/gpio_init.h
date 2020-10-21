/*
 * gpio_init.h
 *
 *  Created on: 13 Nov 2019
 *      Author: s1808875
 */

#ifndef GPIO_INIT_H_
#define GPIO_INIT_H_

#include "xgpio.h"

XStatus initGpio(void);

XGpio SLIDE_SWITCHES;
XGpio P_BTN_UP;
XGpio LED_OUT;
XGpio SEG7_HEX_OUT;
XGpio SEG7_SEL_OUT;

XGpio REGION_0_COLOUR;
XGpio REGION_1_COLOUR;
XGpio REGION_2_COLOUR;
XGpio REGION_3_COLOUR;
XGpio REGION_4_COLOUR;
XGpio REGION_5_COLOUR;
XGpio REGION_6_COLOUR;
XGpio REGION_7_COLOUR;
XGpio REGION_8_COLOUR;
XGpio REGION_9_COLOUR;
XGpio REGION_10_COLOUR;
XGpio REGION_11_COLOUR;
XGpio REGION_12_COLOUR;

#endif /* GPIO_INIT_H_ */