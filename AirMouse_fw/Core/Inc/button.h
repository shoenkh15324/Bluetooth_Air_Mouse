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
#include "usart.h"

#define BUTTON_MAX_CH     2



#define RIGHT_BTN_Pin GPIO_PIN_3
#define RIGHT_BTN_GPIO_Port GPIOB
#define LEFT_BTN_Pin GPIO_PIN_4
#define LEFT_BTN_GPIO_Port GPIOB


typedef struct
{
  GPIO_TypeDef *port; // GPIO Port
  uint32_t pin;       // GPIO Pin
} button_tbl_t;


void buttonInit();
uint8_t isButtonPressed(GPIO_TypeDef *port, uint32_t pin);
void buttonRead(GPIO_TypeDef *port, uint32_t pin);
void cliButton(cli_args_t *args);

#endif /* INC_BUTTON_H_ */
