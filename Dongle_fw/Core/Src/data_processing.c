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
uint32_t cnt_usb = 0;
uint32_t cnt_uart = 0;

bool receiveData(void)
{
  if (HAL_UART_Receive(&huart2, (uint8_t *)HID_report, sizeof(HID_report), 100) == HAL_OK)
    return 1;

  return 0;
}

void sendHIDReport(void)
{
  USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)HID_report, sizeof(HID_report));
}