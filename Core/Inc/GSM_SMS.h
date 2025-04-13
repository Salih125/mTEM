/*
 * GSM_SMS.h
 *
 *  Created on: Mar 28, 2025
 *      Author: salih
 */

#ifndef INC_GSM_SMS_H_
#define INC_GSM_SMS_H_

#include <stm32g4xx_hal.h>
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

void send_SMS();

void recieve_SMS();

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif /* INC_GSM_SMS_H_ */
