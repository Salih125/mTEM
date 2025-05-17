/*
 * Relay.h
 *
 *  Created on: Apr 28, 2025
 *      Author: salih
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_
#include <string.h>
#include <stdio.h>
#include <stm32g4xx_hal.h>
#include <main.h>

void relayOn(GPIO_TypeDef* port, uint16_t pin);

void relayOff(GPIO_TypeDef* port, uint16_t pin);

#endif /* INC_RELAY_H_ */
