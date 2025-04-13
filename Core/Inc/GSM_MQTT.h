/*
 * GSM_MQTT.h
 *
 *  Created on: Apr 3, 2025
 *      Author: salih
 */

#ifndef INC_GSM_MQTT_H_
#define INC_GSM_MQTT_H_

/*
 * GSM_MQTT.c
 *
 *  Created on: Apr 3, 2025
 *      Author: salih
 */
#include <string.h>
#include <stdio.h>
#include <stm32g4xx_hal.h>
#include <main.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

void SIMTransmit(char *cmd);

void SIMTransmit_TXOnly(const char *cmd);

void SIMReset();

void mqtt_conn();

void mqtt_send();

void mqtt_receive();

void extract_payload(uint8_t *receivedData);
#endif /* INC_GSM_MQTT_H_ */
