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

bool receiveData();
void sendHIDReport();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_DATA_PROCESSING_H_ */
