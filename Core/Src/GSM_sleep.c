/*
 * GSM_sleep.c
 *
 *  Created on: Apr 11, 2025
 *      Author: salih
 */
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
	SIMTransmitSleep("AT+CSCLK=1\r\n");
	HAL_GPIO_WritePin(DTR_Port, DTR_Pin, GPIO_PIN_SET);
	HAL_Delay(30000);

	HAL_GPIO_WritePin(DTR_Port, DTR_Pin, GPIO_PIN_RESET);

	while(1){
		SIMTransmitSleep("AT\r\n");
		if(strstr((char *)buffer_sleep, "OK\r\n")){
			break;
		}
		HAL_Delay(100);
	}

}

