/*
 * dataProcessing.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */


#include <data_processing.h>


USBD_HandleTypeDef hUsbDeviceFS;


// Received HID Data.
int8_t HID_report[4];


bool receiveData()
{
  HAL_UART_Receive(&huart2, (uint8_t *)HID_report, sizeof(HID_report), 100);

  cliPrintf("%d %d %d %d\n", HID_report[0], HID_report[1], HID_report[2], HID_report[3]);

  return 1;
}

void sendHIDReport()
{
  USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)HID_report, sizeof(HID_report));
}
