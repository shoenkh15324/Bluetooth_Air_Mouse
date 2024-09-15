/*
 * button.c
 *
 *  Created on: Sep 15, 2024
 *      Author: mok07
 */


#include "button.h"

typedef struct
{
  GPIO_TypeDef *port;
  uint32_t pin;
} button_tbl_t;

button_tbl_t button_tbl[BUTTON_MAX_CH] =
{
  {BTN_TEST_GPIO_Port, BTN_TEST_Pin}, // button test
};



void buttonInit()
{
  cliAdd("button", cliButton);
}

void cliButton(cli_args_t *args)
{
  bool ret = false;

  if(args->argc == 1 && args->isStr(0, "show"))
  {
    uint8_t str = 0;

    while(cliKeepLoop())
    {
      str = uartRead(CH_USART1);

      if(str == 0x0D) break;

      for(int i=0; i<BUTTON_MAX_CH; i++)
      {
        cliPrintf("%d ", !HAL_GPIO_ReadPin(button_tbl[i].port, button_tbl[i].pin));
      }
      cliPrintf("\n");

      HAL_Delay(50);
    }

    ret = true;
  }

  if(ret != true)
  {
    cliPrintf("button show\n");
  }
}
