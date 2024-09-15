/*
 * cli.h
 *
 *  Created on: Aug 16, 2024
 *      Author: mok07
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_


#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"

#include "usart.h"
//#include "usbd_cdc_if.h"


#define CH_CDC        0
#define CH_USART1     1


#define CLI_CMD_LIST_MAX      16
#define CLI_CMD_NAME_MAX      16

#define CLI_LINE_HIS_MAX      4
#define CLI_LINE_BUF_MAX      64


typedef struct
{
  uint16_t   argc;
  char     **argv;

  int32_t  (*getData)(uint8_t index);
  float    (*getFloat)(uint8_t index);
  char    *(*getStr)(uint8_t index);
  bool     (*isStr)(uint8_t index, char *p_str);
} cli_args_t;


typedef struct
{
  uint32_t in;
  uint32_t out;
  uint32_t len;

  uint8_t *p_buf;
} qbuffer_t;


bool cliInit(void);
bool cliOpen(uint8_t ch, uint32_t baud);
bool cliOpenLog(uint8_t ch, uint32_t baud);
bool cliMain(void);
void cliPrintf(const char *fmt, ...);
bool cliAdd(const char *cmd_str, void (*p_func)(cli_args_t *));
bool cliKeepLoop(void);
uint32_t cliAvailable(void);
uint8_t  cliRead(void);
uint32_t cliWrite(uint8_t *p_data, uint32_t length);

uint32_t cdcAvailable(void);
uint8_t cdcRead(void);
uint32_t cdcWrite(uint8_t *p_data, uint32_t length);
void cdcDataIn(uint8_t rx_data);
uint32_t cdcGetBaud(void);

uint32_t uartAvailable(uint8_t ch);
uint32_t uartPrintf(uint8_t ch, char *fmt, ...);
uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length);
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
uint8_t uartRead(uint8_t ch);
bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length);


#endif /* INC_CLI_H_ */
