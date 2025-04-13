/*
 * GSM_sleep.c
 *
 *  Created on: Apr 11, 2025
 *      Author: salih
 */
//Resets buffer and transmits and receives from uart1
#include <string.h>
#include <stdio.h>
#include <stm32g4xx_hal.h>
#include <main.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

uint8_t buffer_sleep[100] = {0};

void SIMTransmitSleep(char *cmd){
  memset(buffer_sleep,0,sizeof(buffer_sleep));
  HAL_UART_Transmit(&huart1,(uint8_t *)cmd,strlen(cmd),1000);
  HAL_UART_Receive(&huart1, buffer_sleep, sizeof(buffer_sleep), 1000);
}

void sleep(){
	SIMTransmitSleep("AT+CSCLK=1");
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
}

