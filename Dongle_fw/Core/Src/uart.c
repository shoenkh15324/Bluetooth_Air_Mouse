#include "uart.h"

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

static qbuffer_t qbuffer;
static uint8_t rx_buf[256];

bool uartInit(void)
{
   bool ret = true;

   qbufferCreate(&qbuffer, &rx_buf[0], sizeof(rx_buf));

   if (HAL_UART_Receive_DMA(&huart2, (uint8_t *)&rx_buf[0], 256) != HAL_OK)
   {
      ret = false;
   }

   qbuffer.in = qbuffer.len - hdma_usart2_rx.Instance->CNDTR;
   qbuffer.out = qbuffer.in;

   return ret;
}

uint32_t uartAvailable(void)
{
   uint32_t ret = 0;

   qbuffer.in = (qbuffer.len - hdma_usart2_rx.Instance->CNDTR);
   ret = qbufferAvailable(&qbuffer);

   return ret;
}

uint8_t uartRead(void)
{
   uint8_t ret = 0;

   qbufferRead(&qbuffer, &ret, 1);

   return ret;
}

uint32_t uartWrite(uint8_t *p_data, uint32_t length)
{
   uint32_t ret = 0;

   HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, p_data, length, 100);

   if (status == HAL_OK)
   {
      ret = length;
   }

   return ret;
}
