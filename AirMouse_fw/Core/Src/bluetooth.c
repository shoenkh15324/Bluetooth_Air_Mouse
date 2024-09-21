/*
 * bluetooth.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */


#include "bluetooth.h"


uint8_t mac[] = SLAVE_MAC_ADDR;   // Slave Module MAC Address.
uint8_t buf[100];                 // buffer for receive AT command response.


void sendATcommand(char *command)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
  //HAL_UART_Receive(&huart2, buf, sizeof(buf), 200);
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
  HAL_Delay (200);

  uint8_t at_cmd[30];
  snprintf((char *)at_cmd, sizeof(at_cmd), "AT+CO0%s", mac);
  sendATcommand ((char *)&at_cmd);
  HAL_Delay (2000);
}

void bluetoothReconnect()
{
  sendATcommand ("AT+RENEW");
  HAL_Delay (200);
  sendATcommand ("AT+RESET");
  HAL_Delay (200);
  sendATcommand ("AT+ROLE0");
  HAL_Delay (200);
  sendATcommand ("AT+RESET");
  HAL_Delay (200);

  uint8_t at_cmd[30];
  snprintf((char *)at_cmd, sizeof(at_cmd), "AT+CO0%s", mac);
  sendATcommand ((char *)&at_cmd);
  HAL_Delay (2000);
}
