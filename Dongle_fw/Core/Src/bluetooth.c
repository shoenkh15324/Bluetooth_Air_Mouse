/*
 * bluetooth.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */


#include "bluetooth.h"


uint8_t buf[100];


void sendATcommand(char *command)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), 100);
  HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 100);
  //HAL_UART_Receive(&huart2, buf, sizeof(buf), 100);
}

void bluetoothInit()
{
  sendATcommand ("AT");
  HAL_Delay (200);
  sendATcommand ("AT+RENEW");
  HAL_Delay (200);
  sendATcommand ("AT+RESET");
  HAL_Delay (200);
  sendATcommand ("AT+ROLE0");
  HAL_Delay (200);
  sendATcommand ("AT+RESET");
  HAL_Delay (2000);
}

void bluetoothReset()
{
  sendATcommand ("AT+RENEW");
  HAL_Delay (200);
  sendATcommand ("AT+RESET");
  HAL_Delay (200);
  sendATcommand ("AT+ROLE0");
  HAL_Delay (200);
  sendATcommand ("AT+RESET");
  HAL_Delay (2000);
}

