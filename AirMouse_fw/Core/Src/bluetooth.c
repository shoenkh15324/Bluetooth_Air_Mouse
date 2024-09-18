/*
 * bluetooth.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */


#include "bluetooth.h"


uint8_t mac[] = "4006A058653E"; // Slave module MAC address.
uint8_t buf[100];

void send_AT_command(char *command)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
  //HAL_UART_Receive(&huart2, buf, sizeof(buf), 200);
}

void Bluetooth_Init()
{
  send_AT_command ("AT");
  HAL_Delay (200);
  send_AT_command ("AT+RENEW");
  HAL_Delay (200);
  send_AT_command ("AT+RESET");
  HAL_Delay (200);
  send_AT_command ("AT+ROLE0");
  HAL_Delay (200);
  send_AT_command ("AT+RESET");
  HAL_Delay (200);

  uint8_t at_cmd[30];
  snprintf((char *)at_cmd, sizeof(at_cmd), "AT+CO0%s", mac);
  send_AT_command ((char *)&at_cmd);
  HAL_Delay (2000);
}

void Bluetooth_Reconnect()
{
  send_AT_command ("AT+RENEW");
  HAL_Delay (200);
  send_AT_command ("AT+RESET");
  HAL_Delay (200);
  send_AT_command ("AT+ROLE0");
  HAL_Delay (200);
  send_AT_command ("AT+RESET");
  HAL_Delay (200);

  uint8_t at_cmd[30];
  snprintf((char *)at_cmd, sizeof(at_cmd), "AT+CO0%s", mac);
  send_AT_command ((char *)&at_cmd);
  HAL_Delay (2000);
}
