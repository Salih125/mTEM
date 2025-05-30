/*
 * GSM_time.c
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

uint8_t buffer_time[100] = {0};
char timeGSM[10] = {0};

//Resets buffer and transmits and receives from uart1
void SIMTransmitTime(char *cmd){
  memset(buffer_time,0,sizeof(buffer_time));
  HAL_UART_Transmit(&huart1,(uint8_t *)cmd,strlen(cmd),1000);
  HAL_UART_Receive(&huart1, buffer_time, sizeof(buffer_time), 1000);
}

void startGPS(){
	SIMTransmitTime("AT\r\n");
	HAL_Delay(100);
	SIMTransmitTime("AT+CGPS=1\r\n");
	HAL_Delay(60000);

}

void getTime(){
	HAL_Delay(2000);
	SIMTransmitTime("AT+CGPSINFO\r\n");
	HAL_Delay(1000);

	char *start = strstr((char*)buffer_time, "+CGPSINFO:");
	if(start != NULL){
		start = strchr(start, ':');
		if(start != NULL){
			start++;

			char *token;
			int fieldIndex = 0;
			token = strtok(start, ",");

			while(token != NULL){
				fieldIndex++;

				if(fieldIndex == 6){
					strncpy(timeGSM, token, 6);
					timeGSM[4] = '\0';

					HAL_UART_Transmit(&huart3, (uint8_t*)timeGSM, strlen(timeGSM), 100);
					HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n", 2, 100);
					break;
				}
				token = strtok(NULL, ",");
			}
		}
	}else{
		HAL_UART_Transmit(&huart3, (uint8_t*)"GPS info not found\r\n", 21, 100);
	}
}
