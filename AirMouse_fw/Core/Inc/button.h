/*
 * button.h
 *
 *  Created on: Sep 15, 2024
 *      Author: mok07
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "gpio.h"
#include "cli.h"

#define BUTTON_MAX_CH     1

#define BTN_TEST_Pin GPIO_PIN_4
#define BTN_TEST_GPIO_Port GPIOB

void buttonInit();

void cliButton(cli_args_t *args);

#endif /* INC_BUTTON_H_ */
