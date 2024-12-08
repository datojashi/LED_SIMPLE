/*
 * wsc1228b.h
 *
 *  Created on: Nov 13, 2024
 *      Author: dato
 */

#ifndef INC_WSC1228B_H_
#define INC_WSC1228B_H_

#include "main.h"

#define LED_COUNT 1
#define LED_CODE_SIZE 3
#define LED_BUF_SIZE (LED_CODE_SIZE*8)


void wsc1228_init_module(TIM_HandleTypeDef* htim);
HAL_StatusTypeDef wsc1228_start_data_sending();
HAL_StatusTypeDef wsc1228_stop_data_sending();
void wsc1228_set_color(int c);

void transferCmplt();
void transferHalfCmplt();
void transferError();

#endif /* INC_WSC1228B_H_ */
