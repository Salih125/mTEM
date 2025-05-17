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
#include <main.h>

#define BATTERY_ADDR (0x0B << 1)

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;

extern float temp1;
extern float temp2;
extern uint16_t batt1;
extern uint16_t batt2;

void batt_charge1();
uint16_t batt_voltage1();
void batt_temp1();

void batt_charge2();
uint16_t batt_voltage2();
void batt_temp2();

#endif /* INC_RRC_BATT_H_ */
