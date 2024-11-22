/*
 * dataProcessing.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#include "data_processing.h"
#include "uart.h"

USBD_HandleTypeDef hUsbDeviceFS;

// Received HID Data.
int8_t HID_report[4];
uint32_t cnt_usb = 0;
uint32_t cnt_uart = 0;

uint32_t dataPadding(int8_t *arr)
{
  uint32_t value = 0;

  value |= ((uint32_t)arr[1] << 24);
  value |= ((uint32_t)arr[2] << 16);
  value |= ((uint32_t)arr[3] << 8);
  value |= ((uint32_t)arr[4] << 0);

  return value;
}

bool validatePacket(int8_t *packet)
{
  // Check Start and End bytes
  if (packet[0] != 0x0B || packet[6] != 0x0E)
    return false;

  // Check CRC
  uint32_t padded_value = dataPadding(packet);
  uint32_t crc_value = HAL_CRC_Calculate(&hcrc, &padded_value, 1);

  return (crc_value == packet[5]);
}

bool receiveData(void)
{
  int8_t temp_buf[7];

  if (uartAvailable() == 0)
    return false;

  for (int i = 0; i < sizeof(temp_buf); i++)
  {
    temp_buf[i] = (int8_t)uartRead();
  }

  if (validatePacket(temp_buf))
  {
    HID_report[0] = temp_buf[1];
    HID_report[1] = temp_buf[2];
    HID_report[2] = temp_buf[3];
    HID_report[3] = temp_buf[4];
    return true;
  }

  return false;
}

void sendHIDReport(void)
{
  USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)HID_report, sizeof(HID_report));
}