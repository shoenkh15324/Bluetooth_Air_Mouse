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


#define SLAVE_MAC_ADDR  "4006A058653E" // Slave Module MAC Address


void sendATcommand(char *command);
void bluetoothInit();
void bluetoothReconnect();

#endif /* INC_BLUETOOTH_H_ */
