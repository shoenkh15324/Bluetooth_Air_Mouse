/*
 * dataProcessing.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */


#include "dataProcessing.h"


USBD_HandleTypeDef hUsbDeviceFS;

int8_t report[4];


bool receiveData()
{
  HAL_UART_Receive(&huart2, (uint8_t *)report, sizeof(report), 100);

  //parse_hid_report((char *)temp_report);

  cliPrintf("%d %d %d %d\n", report[0], report[1], report[2], report[3]);

  return 1;
}

void dataToHID()
{
  USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)report, sizeof(report));
}
