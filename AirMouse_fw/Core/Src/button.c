/*
 * button.c
 *
 *  Created on: Sep 15, 2024
 *      Author: mok07
 */

#include "button.h"

#define DEBOUNCE_DELAY 15

static uint8_t isLeftButtonPressed = 0;
static uint8_t isRightButtonPressed = 0;

// Using for button debouncing.
static uint32_t lastDebounceTime_LEFT = 0;
static uint32_t lastDebounceTime_RIGHT = 0;

void buttonInit()
{
}

uint8_t isButtonPressed(GPIO_TypeDef *port, uint32_t pin)
{
  if (port == LEFT_BTN_GPIO_Port && pin == LEFT_BTN_Pin)
  {
    if (isLeftButtonPressed)
    {
      isLeftButtonPressed = 0;
      return 1;
    }
  }
  else if (port == RIGHT_BTN_GPIO_Port && pin == RIGHT_BTN_Pin)
  {
    if (isRightButtonPressed)
    {
      isRightButtonPressed = 0;
      return 1;
    }
  }

  return 0;
}

void buttonRead(GPIO_TypeDef *port, uint32_t pin)
{
  uint32_t currentTime = HAL_GetTick();

  if (pin == LEFT_BTN_Pin)
  {
    // left button debouncing.
    if ((currentTime - lastDebounceTime_LEFT) > DEBOUNCE_DELAY)
    {
      lastDebounceTime_LEFT = currentTime;

      if (HAL_GPIO_ReadPin(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin) == GPIO_PIN_SET)
      {
        isLeftButtonPressed = 1;
      }
    }
    /// right button debouncing.
    else if (pin == RIGHT_BTN_Pin)
    {
      if ((currentTime - lastDebounceTime_RIGHT) > DEBOUNCE_DELAY)
      {
        lastDebounceTime_RIGHT = currentTime;

        if (HAL_GPIO_ReadPin(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin) == GPIO_PIN_SET)
        {
          isRightButtonPressed = 1;
        }
      }
    }
  }
}
