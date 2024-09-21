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

#include "cli.h"
#include "usb_device.h"
#include "usbd_hid.h"


bool receiveData();
void sendHIDReport();

//uint8_t USBD_HID_SendReport(USBD_HandleTypeDef  *pdev, uint8_t *report, uint16_t len);


#endif /* INC_DATA_PROCESSING_H_ */
