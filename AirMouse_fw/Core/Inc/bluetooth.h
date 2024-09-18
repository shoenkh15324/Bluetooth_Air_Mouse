/*
 * bluetooth.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "usart.h"

void send_AT_command(char *command);
void Bluetooth_Init();
void Bluetooth_Reconnect();

#endif /* INC_BLUETOOTH_H_ */
