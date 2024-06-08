/*
 * usart1.c
 *
 *  Created on: June 07, 2024
 *      Author: SHUAIWEN CUI
 */

#include "iusart.h"

uint8_t USART1_RX_BUF[USART1_REC_LEN];// receive buffer, up to USART_REC_LEN bytes.
uint16_t USART1_RX_STA=0;// receive status flag//bit15: receive completion flag, bit14: receive 0x0d, bit13~0: number of valid bytes received
uint8_t USART1_NewData;// cache for 1 byte of data received by the current serial port

// uint8_t USART2_RX_BUF[USART2_REC_LEN];// receive buffer, up to USART_REC_LEN bytes.
// uint16_t USART2_RX_STA=0;// receive status flag//bit15: receive completion flag, bit14: receive 0x0d, bit13~0: number of valid bytes received
// uint8_t USART2_NewData;// cache for 1 byte of data received by the current serial port

// uint8_t USART3_RX_BUF[USART3_REC_LEN];// receive buffer, up to USART_REC_LEN bytes.
// uint16_t USART3_RX_STA=0;// receive status flag//bit15: receive completion flag, bit14: receive 0x0d, bit13~0: number of valid bytes received
// uint8_t USART3_NewData;// cache for 1 byte of data received by the current serial port

// uint8_t UART4_RX_BUF[UART4_REC_LEN];// receive buffer, up to USART_REC_LEN bytes.
// uint16_t UART4_RX_STA=0;// receive status flag//bit15: receive completion flag, bit14: receive 0x0d, bit13~0: number of valid bytes received
// uint8_t UART4_NewData;// cache for 1 byte of data received by the current serial port

// uint8_t USART6_RX_BUF[USART6_REC_LEN];// receive buffer, up to USART_REC_LEN bytes.
// uint16_t USART6_RX_STA=0;// receive status flag//bit15: receive completion flag, bit14: receive 0x0d, bit13~0: number of valid bytes received
// uint8_t USART6_NewData;// cache for 1 byte of data received by the current serial port

//USB serial communication, use the printf function of USART1
//Usage: USART1_printf("123"); //Send character 123 to USART1
void USART1_printf(char *fmt, ...)
{
    char buff[USART1_REC_LEN+1];  //Used to store converted data [length]
    uint16_t i=0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(buff, USART1_REC_LEN+1, fmt,  arg_ptr);//data conversion
    i=strlen(buff);//get the length of the data
    if(strlen(buff)>USART1_REC_LEN)i=USART1_REC_LEN;//If the length is greater than the maximum value, the length is equal to the maximum value (the excess part is ignored)
    HAL_UART_Transmit(&huart1,(uint8_t  *)buff,i,0xffff);//Serial port transmission function (serial port number, content, quantity, overflow time)
    va_end(arg_ptr);
}

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart)//Serial port interrupt callback function
{
	if(huart ==&huart1)//Determine the source of the interrupt (serial port 1: USB to serial port)
    {
		USART1_printf("%c",USART1_NewData); //Send the received data back to the computer with the a symbol variable
       if((USART1_RX_STA&0x8000)==0){//Reception is not complete
           if(USART1_RX_STA&0x4000){//Received 0x0d
               if(USART1_NewData!=0x0a)USART1_RX_STA=0;//Reception error, restart
               else USART1_RX_STA|=0x8000;   //Reception is complete
           }else{ //Haven't received 0X0D
               if(USART1_NewData==0x0d)USART1_RX_STA|=0x4000;
               else{
                  USART1_RX_BUF[USART1_RX_STA&0X3FFF]=USART1_NewData; //Put the received data into the array
                  USART1_RX_STA++;  //Data length count plus 1
                  if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//Receiving data error, start receiving again
               }
           }
       }
       HAL_UART_Receive_IT(&huart1,(uint8_t *)&USART1_NewData,1); //Enable receive interrupt
    }
	// if(huart ==&huart2)//Determine the source of the interrupt (serial port 2: WIFI module)
	// {
	// 	if(USART2_RX_STA<USART2_REC_LEN)//More data can be received
	// 	{
	// 		__HAL_TIM_SET_COUNTER(&htim2,0); //Counter cleared
	// 		if(USART2_RX_STA==0) //Enable timer 2 interrupt
	// 		{
	// 			__HAL_TIM_ENABLE(&htim2); //Enable timer 2
	// 		}
	// 		USART2_RX_BUF[USART2_RX_STA++] = USART2_NewData;//The latest received data is placed in the array
	// 	}else{
	// 		USART2_RX_STA|=0x8000;//Force mark reception completion
	// 	}
	// 	HAL_UART_Receive_IT(&huart2,(uint8_t *)&USART2_NewData,1); //Enable serial port 3 receive interrupt again
	// }
    // if(huart ==&huart3)// Determine the source of the interrupt (Bluetooth module)
    // {
	// 	if((USART3_RX_STA&0x8000)==0){// Receiving is not complete (the highest bit of USART3_RX_STA is defined as the receive completion flag)
	// 	   if(USART3_NewData==0x0A)//If 0x0A is received, it means that the end symbol is received (the Bluetooth module replies with 0x0A as the end symbol)
	// 	   {
	// 		   USART3_RX_STA|=0x8000;//Received 0x0A, accepted
	// 	   }
	// 	   else{ //If 0x0A is not received, continue to receive the data content and add the quantity by 1
	// 		  USART3_RX_BUF[USART3_RX_STA&0X7FFF]=USART3_NewData; //Put the received data into the array
	// 		  USART3_RX_STA++;  //Data length count plus 1
	// 		  if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//Receiving data error, start receiving again
	// 	   }
	// 	}
	// 	HAL_UART_Receive_IT(&huart3,(uint8_t *)&USART3_NewData,1); //Enable receive interrupt again
    // }
    // if(huart ==&huart4)// Determine the source of the interrupt (RS485)
    // {
    // 	UART4_RX_BUF[0]=UART4_NewData;//Put the received data into the cache array (because only one data is used, it is only stored in the data [0] position)
    // 	UART4_RX_STA++;//Data reception flag plus 1
    // 	HAL_UART_Receive_IT(&huart4,(uint8_t *)&UART4_NewData, 1); //Enable receive interrupt
    // }
}
