/*
 * iusart.h
 *
 *  Created on: June 07, 2024
 *      Author: SHUAIWEN CUI
 */

#ifndef IUSART_H_
#define IUSART_H_

#include "stm32h7xx_hal.h" // HAL library file declaration, replace it with the corresponding file according to the actual situation
#include <string.h> // Library for string processing
#include <stdarg.h>
#include <stdlib.h>
#include "stdio.h"
// #include "../tim/tim.h"

extern UART_HandleTypeDef huart1;// Declare the HAL library structure of USART1
// extern UART_HandleTypeDef huart2;// Declare the HAL library structure of USART2
// extern UART_HandleTypeDef huart3;// Declare the HAL library structure of USART3
// extern UART_HandleTypeDef huart4;// Declare the HAL library structure of USART4
// extern UART_HandleTypeDef huart6;// Declare the HAL library structure of USART6

#define USART1_REC_LEN  200 // Define the maximum number of bytes received by USART1
// #define USART2_REC_LEN  200 // Define the maximum number of bytes received by USART2
// #define USART3_REC_LEN  200 // Define the maximum number of bytes received by USART3
// #define UART4_REC_LEN   200 // Define the maximum number of bytes received by UART4
// #define USART6_REC_LEN  200 // Define the maximum number of bytes received by USART6

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];// Receive buffer, up to USART_REC_LEN bytes. The last byte is a newline character
extern uint16_t USART1_RX_STA;// Receive status flag
extern uint8_t USART1_NewData;// Cache for 1 byte of data received by the current serial port

// extern uint8_t  USART2_RX_BUF[USART2_REC_LEN];// Receive buffer, up to USART_REC_LEN bytes. The last byte is a newline character
// extern uint16_t USART2_RX_STA;// Receive status flag
// extern uint8_t USART2_NewData;// Cache for 1 byte of data received by the current serial port

// extern uint8_t  USART3_RX_BUF[USART3_REC_LEN];// Receive buffer, up to USART_REC_LEN bytes. The last byte is a newline character
// extern uint16_t USART3_RX_STA;// Receive status flag
// extern uint8_t USART3_NewData;// Cache for 1 byte of data received by the current serial port

// extern uint8_t  UART4_RX_BUF[UART4_REC_LEN];// Receive buffer, up to USART_REC_LEN bytes. The last byte is a newline character
// extern uint16_t UART4_RX_STA;// Receive status flag
// extern uint8_t UART4_NewData;// Cache for 1 byte of data received by the current serial port

// extern uint8_t  USART6_RX_BUF[USART6_REC_LEN];// Receive buffer, up to USART_REC_LEN bytes. The last byte is a newline character
// extern uint16_t USART6_RX_STA;// Receive status flag
// extern uint8_t USART6_NewData;// Cache for 1 byte of data received by the current serial port

void USART1_printf(char *fmt, ...);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart); // Declaration of serial port interrupt callback function

#endif /* IUSART_H_ */
