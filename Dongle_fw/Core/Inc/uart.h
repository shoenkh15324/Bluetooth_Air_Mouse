#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "usart.h"
#include "qbuffer.h"

bool uartInit(void);
uint32_t uartAvailable(void);
uint8_t uartRead(void);
uint32_t uartWrite(uint8_t *p_data, uint32_t length);

#endif