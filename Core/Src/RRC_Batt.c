/*
 * RRCbat.c
 *
 *  Created on: Mar 27, 2025
 *      Author: salih
 */

#include <stm32g4xx_hal.h>
#include <stdio.h>
#include <string.h>

#define BATTERY_ADDR (0x0B << 1)

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;

uint8_t command_charge = 0x0D;
uint8_t command_voltage = 0x09;
uint8_t buffer[2];
char uart_buffer[27];

// function to get the battery capacity in % on first battery
float batt_charge1(){

	HAL_Delay(20);

	if (HAL_I2C_Master_Transmit(&hi2c1, BATTERY_ADDR, &command_charge, 1, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 TX Failed!\r\n", 19, 100);
	}

	if (HAL_I2C_Master_Receive(&hi2c1, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 RX Failed!\r\n", 19, 100);
	}

	uint16_t charge = (buffer[1] << 8) | buffer[0];  // Converts little-endian

	sprintf(uart_buffer, "Battery Charge1: %d%% \r\n", charge);
	HAL_UART_Transmit(&huart1, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);

	return charge;
}

// function to get the voltage of the first battery in mV
float batt_voltage1(){

    HAL_Delay(20);

    if (HAL_I2C_Master_Transmit(&hi2c1, BATTERY_ADDR, &command_voltage, 1, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 TX Failed!\r\n", 19, 100);
    }

    if (HAL_I2C_Master_Receive(&hi2c1, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 RX Failed!\r\n", 19, 100);
    }

    uint16_t voltage = (buffer[1] << 8) | buffer[0];  // Converts little-endian

    sprintf(uart_buffer, "Voltage1: %dmv\r\n", voltage);
    HAL_UART_Transmit(&huart1, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);

    return voltage;
}

// function to get the battery capacity in % on second battery
float batt_charge2(){

	HAL_Delay(20);

	if (HAL_I2C_Master_Transmit(&hi2c2, BATTERY_ADDR, &command_charge, 1, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C2 TX Failed!\r\n", 19, 100);
	}

	if (HAL_I2C_Master_Receive(&hi2c2, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C2 RX Failed!\r\n", 19, 100);
	}

	uint16_t charge = (buffer[1] << 8) | buffer[0];  // Converts little-endian

	sprintf(uart_buffer, "Battery Charge2: %d%% \r\n", charge);
	HAL_UART_Transmit(&huart1, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);

	return charge;
}

// function to get the voltage of the second battery in mV
float batt_voltage2(){

    HAL_Delay(20);

    if (HAL_I2C_Master_Transmit(&hi2c2, BATTERY_ADDR, &command_voltage, 1, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C2 TX Failed!\r\n", 19, 100);
    }

    if (HAL_I2C_Master_Receive(&hi2c2, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C2 RX Failed!\r\n", 19, 100);
    }

    uint16_t voltage = (buffer[1] << 8) | buffer[0];  // Converts little-endian

    sprintf(uart_buffer, "Voltage2: %dmv\r\n", voltage);
    HAL_UART_Transmit(&huart1, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);

    return voltage;
}
