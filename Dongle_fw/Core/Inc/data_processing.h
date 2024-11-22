/*
 * dataProcessing.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_DATA_PROCESSING_H_
#define INC_DATA_PROCESSING_H_

#include <stdbool.h>
#include <string.h>

#include "usart.h"
#include "usb_device.h"
#include "usbd_hid.h"
#include "crc.h"

uint32_t dataPadding(int8_t *HID_report);
bool validatePacket(int8_t *packet);
bool receiveData();
void sendHIDReport();

#endif /* INC_DATA_PROCESSING_H_ */
