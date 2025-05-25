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

void extract_payload(uint8_t *receivedData);

const char apn[]  = "internet"; 
const char host[] = "tcp://test.mosquitto.org"; 
const int  port = 1883;
const char topic[] = "sim";
char ATcommand_MQTT[100];
uint8_t buffer_MQTT[100] = {0};
char charData[15] = "send test";
char payload[100] = {0};
uint8_t receivedData[100] = {0};
int count = 0;
int received = 0;

//Resets buffer and transmits and receives from uart1
void SIMTransmit(char *cmd){
  memset(buffer_MQTT,0,sizeof(buffer_MQTT));
  HAL_UART_Transmit(&huart1,(uint8_t *)cmd,strlen(cmd),1000);
  HAL_UART_Receive(&huart1, buffer_MQTT, sizeof(buffer_MQTT), 1000);
}

//Transmits from uart1
void SIMTransmit_TXOnly(const char *cmd) {
    HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), 1000);
    HAL_Delay(100);
}

//Resets SIM7600
void SIMReset(){
	SIMTransmit("AT+CRESET\r\n");
}

void extract_payload(uint8_t *receivedData) {
    char *ptr = strstr((char *)receivedData, "+CMQTTRXPAYLOAD:"); // searching for "+CMQTTRXPAYLOAD:"
    if (ptr != NULL) {
        ptr = strchr(ptr, '\n');
        if (ptr != NULL) {
            ptr++; 
            char *end = strchr(ptr, '\r'); 
            if (end != NULL) {
                size_t len = end - ptr;
                if (len < sizeof(payload)) {
                    strncpy(payload, ptr, len);
                    payload[len] = '\0';
                }
            }
        }
    }
}

void mqtt_conn(){
    // Check SIM registration status
    SIMTransmit("AT+CREG?\r\n");

    // PDP context setup
	sprintf(ATcommand_MQTT,"AT+CGSOCKCONT=1,\"IP\",\"%s\"\r\n",apn);
	SIMTransmit(ATcommand_MQTT);

	// Start MQTT service
	SIMTransmit("AT+CMQTTSTART\r\n"); // Start MQTT Service
	// Acquire client
	SIMTransmit("AT+CMQTTACCQ=0,\"mqttclient\"\r\n"); // Acquire a Client

	// Connect to a MQTT Server
	sprintf(ATcommand_MQTT,"AT+CMQTTCONNECT=0,\"%s:%d\",60,1\r\n",host,port);
	SIMTransmit(ATcommand_MQTT);
}

void mqtt_receiveconn(){
	SIMTransmit("AT+CMQTTDISC=0,120\r\n");
	// Release client and stop service
	SIMTransmit("AT+CMQTTREL=0\r\n"); // Release the Client
	SIMTransmit("AT+CMQTTSTOP\r\n"); // Stop MQTT Service

	// Start MQTT service
	SIMTransmit("AT+CMQTTSTART\r\n"); // Start MQTT Service
	// Acquire client
	SIMTransmit("AT+CMQTTACCQ=0,\"mqttclient\"\r\n"); // Acquire a Client

	// Connect to a MQTT Server
	sprintf(ATcommand_MQTT,"AT+CMQTTCONNECT=0,\"%s:%d\",60,1\r\n",host,port);
	SIMTransmit(ATcommand_MQTT);

    // subscribe and set topic
    sprintf(ATcommand_MQTT, "AT+CMQTTSUB=0,%d,1\r\n", (int)strlen(topic));
    SIMTransmit(ATcommand_MQTT);
    sprintf(ATcommand_MQTT, "%s\r\n", topic);
    SIMTransmit_TXOnly(ATcommand_MQTT);

}

void mqtt_disconn(){
		SIMTransmit("AT+CMQTTDISC=0,120\r\n");
		// Release client and stop service
		SIMTransmit("AT+CMQTTREL=0\r\n"); // Release the Client
		SIMTransmit("AT+CMQTTSTOP\r\n"); // Stop MQTT Service
}


void mqtt_send(){
	mqtt_conn();

	// Set the topic for publish message
	sprintf(ATcommand_MQTT,"AT+CMQTTTOPIC=0,%d\r\n",strlen(topic));
	SIMTransmit(ATcommand_MQTT);
	sprintf(ATcommand_MQTT,"%s\r\n",topic);
	SIMTransmit(ATcommand_MQTT);

	// Set payload
	sprintf(ATcommand_MQTT,"AT+CMQTTPAYLOAD=0,%d\r\n",strlen(charData));
	SIMTransmit(ATcommand_MQTT);
	SIMTransmit(charData);

	// Publish message
	SIMTransmit("AT+CMQTTPUB=0,1,60\r\n");
	// Disconnect from MQTT server
	SIMTransmit("AT+CMQTTDISC=0,120\r\n");
	// Release client and stop service
	SIMTransmit("AT+CMQTTREL=0\r\n"); // Release the Client
	SIMTransmit("AT+CMQTTSTOP\r\n"); // Stop MQTT Service
}

void mqtt_receive(void) {
    while (count<=1) {
    	memset(receivedData, 0, sizeof(receivedData));
    	HAL_UART_Receive(&huart1, receivedData, sizeof(receivedData), 5000);

    	extract_payload(receivedData);

    	if (strlen(payload) > 0) {
    		HAL_UART_Transmit(&huart3, (uint8_t*)payload, strlen(payload), 100);
        	count = count +1;
    		memset(payload, 0, sizeof(payload));
    	//  HAL_UART_Transmit(&huart3, (uint8_t*)"\r\n", 2, 100);
    	}
}
    count = 0;
}




