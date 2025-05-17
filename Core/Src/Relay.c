/*
 * Relay.c
 *
 *  Created on: Apr 28, 2025
 *      Author: salih
 */

#include <string.h>
#include <stdio.h>
#include <stm32g4xx_hal.h>
#include <main.h>

void relayOn(GPIO_TypeDef* port, uint16_t pin){
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void relayOff(GPIO_TypeDef* port, uint16_t pin){
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}
