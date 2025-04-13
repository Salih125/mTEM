/*
 * GSM_time.h
 *
 *  Created on: Apr 11, 2025
 *      Author: salih
 */

#ifndef INC_GSM_TIME_H_
#define INC_GSM_TIME_H_

#include <string.h>
#include <stdio.h>
#include <stm32g4xx_hal.h>
#include <main.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;


void SIMTransmitTime(char *cmd);

void date_time();

#endif /* INC_GSM_TIME_H_ */
