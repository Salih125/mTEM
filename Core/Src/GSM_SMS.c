/*
 * GSM.c
 *
 *  Created on: Mar 28, 2025
 *      Author: salih
 */
#include <stm32g4xx_hal.h>
#include <string.h>
#include <stdio.h>
#include <main.h>

#define PREF_SMS_STORAGE "\"SM\""

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

char ATcommand[80];
uint8_t GSM_buffer[30] = {0};
uint8_t ATisOK = 0;
uint8_t slot = 0;
uint8_t rx_index = 0;
uint8_t rx_data;
// Global variables for time control
char recvTime[10] = {0};

// Resets the GSM module
void reset_GSM(){
    sprintf(ATcommand,"AT+CRESET\r\n");
    HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand,strlen(ATcommand), 1000);
}

// Deletes all SMS messages sent
void delete_SMS(){
    sprintf(ATcommand,"AT+CMGD=,4\r\n");
    HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand,strlen(ATcommand), 1000);
}

void send_SMS(char mobileNumber[], char message[]){

	// Transmits AT until module returns "OK"
	while(!ATisOK){
		sprintf(ATcommand,"AT\r\n");
		HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand, strlen(ATcommand),100);
		HAL_UART_Receive_IT(&huart1, GSM_buffer, sizeof(GSM_buffer));
		HAL_Delay(1000);
		if(strstr((char *)GSM_buffer,"OK")){
			ATisOK = 1;
		}
		HAL_Delay(1000);
		memset(GSM_buffer, 0, sizeof(GSM_buffer));
	}

	// Sets SMS format to text-mode
	sprintf(ATcommand,"AT+CMGF=1\r\n");
	HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand, strlen(ATcommand),1000);
	HAL_UART_Receive_IT(&huart1, GSM_buffer, sizeof(GSM_buffer));
	HAL_Delay(1000);
	memset(GSM_buffer,0,sizeof(GSM_buffer));

	// Sets the phone number to send a message to
	sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n", mobileNumber);
	HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand, strlen(ATcommand),1000);
	HAL_Delay(100);

	// Transmits message and 0x1a which is ASCII for CTRL+Z
	sprintf(ATcommand, "%s%c", message, 0x1a);
	HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand, strlen(ATcommand),1000);
	HAL_UART_Receive_IT(&huart1, GSM_buffer, sizeof(GSM_buffer));
	memset(GSM_buffer,0, sizeof(GSM_buffer));
	HAL_Delay(1000);
}

void receive_SMS(){
	while(!ATisOK){
		sprintf(ATcommand,"AT\r\n");
		HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand, strlen(ATcommand),100);
		HAL_UART_Receive (&huart1, GSM_buffer, sizeof(GSM_buffer), 100);
		HAL_Delay(1000);
		if(strstr((char *)GSM_buffer,"OK")){
			ATisOK = 1;
		}
		HAL_Delay(1000);
		memset(GSM_buffer,0,sizeof(GSM_buffer));
	}
	// Sets SMS format to text-mode
	sprintf(ATcommand, "AT+CMGF=1\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
	HAL_UART_Receive(&huart1, GSM_buffer, 100, 100);
	HAL_Delay(1000);
	memset(GSM_buffer, 0, sizeof(GSM_buffer));

	// Sets the SMS storage to directly UART
	sprintf(ATcommand, "AT+CNMI=2,1\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand),1000);
	HAL_UART_Receive_IT(&huart1, GSM_buffer, 1);
	HAL_UART_Transmit(&huart1, (uint8_t *)GSM_buffer, sizeof(GSM_buffer),1000);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(huart->Instance==USART1)
  {
    // if the character received is other than 'enter' ascii13, save the data in buffer
    if(rx_data!=13)
    {
    	GSM_buffer[rx_index++]=rx_data;
    }
    else
    {
      // if new message arrived, read the message
      if( sscanf((char*)GSM_buffer, "\n+CMTI: " PREF_SMS_STORAGE ",%hhd", &slot)==1)
      {
        sprintf(ATcommand,"AT+CMGR=%d\r\n",slot);
        HAL_UART_Transmit_IT(&huart1,(uint8_t *)ATcommand, strlen(ATcommand));
      }
      // if message read contains "ledon", switch the LED ON
      else if (strstr((char *)GSM_buffer,"LEDON"))
      {
//        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    	  HAL_GPIO_WritePin(RELAY1_Port, RELAY1_Pin, GPIO_PIN_SET);


      }
      // if message read contains "ledoff", switch the LED OFF
      else if (strstr((char *)GSM_buffer,"LEDOFF"))
      {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
      }
      // This will delete all messages in the SIM card)
      else if (strstr((char *)GSM_buffer,"DELALL"))
      {
          sprintf(ATcommand,"AT+CMGD=,4\r\n");
          HAL_UART_Transmit(&huart1,(uint8_t *)ATcommand,strlen(ATcommand), 1000);
      }
      else if(strstr((char *)GSM_buffer,"TIME:")){
    	  char* token = strchr(GSM_buffer, ':');
    	  if (token != NULL && strlen(token) >= 5) {
    		  strncpy(recvTime, token + 1, 4);
    		  recvTime[4] = '\0';
    	  }
      }
      rx_index=0;
      memset(GSM_buffer,0,sizeof(GSM_buffer));
    }
    // Enabling interrupt receive again
    HAL_UART_Receive_IT(&huart1,&rx_data,1); // receive data (one character only)
  }
}
