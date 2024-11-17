/**
  ******************************************************************************
  * @file           : uart_logger.h
  * @brief          : Header for uart_logger.c file.
  *                   This file contains the common defines and function declarations for logging throw UART.
  ******************************************************************************
 */

#ifndef __UART_LOGGER_H
#define __UART_LOGGER_H

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"



extern uint8_t loging;

void uart_logger_init(UART_HandleTypeDef* _phuart);

void uart_log(char* format, ... );
void uart_recv(char* data, size_t len);
void printHex(char* data, int size);

#define __LOG__

#ifdef __LOG__
#define LOG(format, ... )  if(loging) uart_log(format, ##__VA_ARGS__)
#else
#define LOG(format, ... )
#endif



#endif
