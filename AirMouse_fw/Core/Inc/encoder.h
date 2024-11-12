#ifndef INC_ENCODER_H
#define INC_ENCODER_H

#include <stdio.h>
#include <stdbool.h>

#include "gpio.h"
#include "tim.h"

void encoderInit();
void readEncoderSwitch(GPIO_TypeDef *port, uint32_t pin);

bool getIsSwitch();
void setIsSwitchTrue();
void setIsSwitchFalse();

#endif
