/*
 * RRCbat.c
 *
 *  Created on: Mar 27, 2025
 *      Author: salih
 */

#include <stm32g4xx_hal.h>
#include <stdio.h>
#include <string.h>
#include <main.h>

#define BATTERY_ADDR (0x0B << 1)

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

uint8_t command_charge = 0x0D;
uint8_t command_voltage = 0x09;
uint8_t commant_temp =0x08;
uint8_t buffer[2];
char uart_buffer[27];

float temp1 = 0;
float temp2 = 0;
uint16_t batt1 = 0;
uint16_t batt2 = 0;

// function to get the battery capacity in % on first battery
void batt_charge1(){

	relayOn(RELAY2_Port, RELAY2_Pin);
	relayOff(RELAY3_Port, RELAY3_Pin);
	HAL_Delay(500);

	if (HAL_I2C_Master_Transmit(&hi2c1, BATTERY_ADDR, &command_charge, 1, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 TX Failed!\r\n", 19, 100);
	}

	if (HAL_I2C_Master_Receive(&hi2c1, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 RX Failed!\r\n", 19, 100);
	}

	uint16_t charge = (buffer[1] << 8) | buffer[0];  // Converts little-endian
	batt1 = charge;

	sprintf(uart_buffer, "Battery Charge1: %d%% \r\n", charge);
	HAL_UART_Transmit(&huart3, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);

}

// function to get the voltage of battery 1 in mV
uint16_t batt_voltage1(){

	relayOn(RELAY2_Port, RELAY2_Pin);
	relayOff(RELAY3_Port, RELAY3_Pin);
	HAL_Delay(500);

    if (HAL_I2C_Master_Transmit(&hi2c1, BATTERY_ADDR, &command_voltage, 1, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 TX Failed!\r\n", 19, 100);
    }

    if (HAL_I2C_Master_Receive(&hi2c1, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 RX Failed!\r\n", 19, 100);
    }

    uint16_t voltage = (buffer[1] << 8) | buffer[0];  // Converts little-endian

//    sprintf(uart_buffer, "Voltage1: %dmv\r\n", voltage);
//    HAL_UART_Transmit(&huart1, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);

    return voltage;
}

// function the get the temperature from battery 1
void batt_temp1(){

	relayOn(RELAY2_Port, RELAY2_Pin);
	relayOff(RELAY3_Port, RELAY3_Pin);
	HAL_Delay(500);

    if (HAL_I2C_Master_Transmit(&hi2c1, BATTERY_ADDR, &commant_temp, 1, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 TX Failed!\r\n", 19, 100);
    }

    if (HAL_I2C_Master_Receive(&hi2c1, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 RX Failed!\r\n", 19, 100);
    }

    uint16_t temp = (buffer[1] << 8) | buffer[0];  // Converts little-endian
    temp1 = ((float)temp - 273.15f) / 100;

    sprintf(uart_buffer, "Temperature1: %.2f Celcius\r\n", temp1);
    HAL_UART_Transmit(&huart3, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);
}

// function to get the battery capacity in % on second battery
void batt_charge2(){

	relayOn(RELAY3_Port, RELAY3_Pin);
	relayOff(RELAY2_Port, RELAY2_Pin);
	HAL_Delay(500);

	if (HAL_I2C_Master_Transmit(&hi2c2, BATTERY_ADDR, &command_charge, 1, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C2 TX Failed!\r\n", 19, 100);
	}

	if (HAL_I2C_Master_Receive(&hi2c2, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
		HAL_UART_Transmit(&huart1, (uint8_t*)"I2C2 RX Failed!\r\n", 19, 100);
	}

	uint16_t charge = (buffer[1] << 8) | buffer[0];  // Converts little-endian
	batt2 = charge;

	sprintf(uart_buffer, "Battery Charge2: %d%% \r\n", charge);
	HAL_UART_Transmit(&huart3, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);
}

// function to get the voltage of the second battery in mV
uint16_t batt_voltage2(){

	relayOn(RELAY3_Port, RELAY3_Pin);
	relayOff(RELAY2_Port, RELAY2_Pin);
	HAL_Delay(500);

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

// function the get the temperature from battery 2
void batt_temp2(){

	relayOn(RELAY3_Port, RELAY3_Pin);
	relayOff(RELAY2_Port, RELAY2_Pin);
	HAL_Delay(500);

    if (HAL_I2C_Master_Transmit(&hi2c2, BATTERY_ADDR, &commant_temp, 1, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 TX Failed!\r\n", 19, 100);
    }

    if (HAL_I2C_Master_Receive(&hi2c2, BATTERY_ADDR, buffer, 2, 100) != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"I2C1 RX Failed!\r\n", 19, 100);
    }

    uint16_t temp = (buffer[1] << 8) | buffer[0];  // Converts little-endian
    temp2 = ((float)temp - 273.15f) / 100;

    sprintf(uart_buffer, "Temperature2: %.2f Celcius\r\n", temp2);
    HAL_UART_Transmit(&huart3, (uint8_t*)uart_buffer, strlen(uart_buffer), 100);
}


