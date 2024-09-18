/*
 * dataProcessing.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_DATAPROCESSING_H_
#define INC_DATAPROCESSING_H_

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "cli.h"
#include "usb_device.h"


void parse_hid_report(const char *str);
bool receiveData();
void dataToHID();

uint8_t USBD_HID_SendReport(USBD_HandleTypeDef  *pdev,
                            uint8_t *report,
                            uint16_t len);


#endif /* INC_DATAPROCESSING_H_ */
