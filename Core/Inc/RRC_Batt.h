/*
 * RRC_Batt.h
 *
 *  Created on: Mar 27, 2025
 *      Author: salih
 */

#ifndef INC_RRC_BATT_H_
#define INC_RRC_BATT_H_

#include <stm32g4xx_hal.h>
#include <stdio.h>
#include <string.h>

#define BATTERY_ADDR (0x0B << 1)

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;

float batt_charge1();

float batt_voltage1();

float batt_charge2();

float batt_voltage2();

#endif /* INC_RRC_BATT_H_ */
