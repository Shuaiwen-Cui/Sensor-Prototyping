/*
 * iled.h
 *
 *  Created on: June 07, 2024
 *      Author: SHUAIWEN CUI
 */

#ifndef ILED_H_
#define ILED_H_

#include "stm32h7xx_hal.h" // HAL library file declaration, replace it with the corresponding file according to the actual situation
#include "main.h" // IO definition and initialization function are in the main.c file, must be referenced

#define LED_Pin GPIO_PIN_7

void LED(uint8_t a);// LED1 independent control function (0 is off, other values are on)
void LED_Switch(void);// LED1 state switch

#endif /* ILED_H_ */