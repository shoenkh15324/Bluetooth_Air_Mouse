#include "encoder.h"

bool is_switched = false;

bool curr_state = true;
bool last_state = true;


void encoderInit()
{
     HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

void readEncoderSwitch(GPIO_TypeDef *port, uint32_t pin)
{
     curr_state = !HAL_GPIO_ReadPin(ENCODER_OTS_GPIO_Port, ENCODER_OTS_Pin);

     if(curr_state == 0 && last_state == 1)
     {
       is_switched = !is_switched;

       if(is_switched)
         setIsSwitchTrue();
       else
         setIsSwitchFalse();
     }

     last_state = curr_state;
}

bool getIsSwitch()
{
     return is_switched;
}

void setIsSwitchTrue()
{
     is_switched = true;
}

void setIsSwitchFalse()
{
     is_switched = false;
}
