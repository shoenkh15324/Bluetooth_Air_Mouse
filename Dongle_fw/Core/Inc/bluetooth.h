/*
 * bluetooth.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <stdio.h>

#include "usart.h"
#include "string.h"

void bluetoothInit();
void sendATcommand(char *command);
void bluetoothReset();

#endif /* INC_BLUETOOTH_H_ */
