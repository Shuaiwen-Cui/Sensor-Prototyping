/*
 * iled.c
 *
 *  Created on: June 07, 2024
 *      Author: SHUAIWEN CUI
 */

#include "iled.h"

void LED(uint8_t a)// LED1 independent control function (0 is off, other values are on)
{
	if(a)HAL_GPIO_WritePin(GPIOH,LED_Pin,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOH,LED_Pin,GPIO_PIN_SET);
}


void LED_Toggle(void){
	HAL_GPIO_WritePin(GPIOH,LED_Pin,1-HAL_GPIO_ReadPin(GPIOH,LED_Pin));
}
