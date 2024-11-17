/*
 * wsc1228b.c
 *
 *  Created on: Nov 13, 2024
 *      Author: dato
 */

#include "main.h"
#include "wsc1228b.h"

TIM_HandleTypeDef* htim;

uint16_t led_dma_buffer[LED_BUF_SIZE] = {0};

uint32_t color=0;
uint16_t ccr_0 = 27;
uint16_t ccr_1 = 54;
uint16_t ccr_ret = 80;

uint32_t led_ct=0;

uint8_t colors[8][3]={
		{255,0,0},
		{0,255,0},
		{0,0,255},
		{255,255,0},
		{255,0,255},
		{0,255,255},
		{10,10,10},
		{100,100,100}
};


static void fill_dma_buffer()
{
	uint8_t bit;
	for(int i=0; i<4; i++)
	{
		bit=1;
		for(int j=0; j<8; j++)
		{
			if( i==3 )
			{
				led_dma_buffer[i*4+j]=ccr_ret;
			}
			else if( (colors[color][i]&bit) == 0 )
			{
				led_dma_buffer[i*4+j]=ccr_0;
			}
			else
			{
				led_dma_buffer[i*4+j]=ccr_1;
			}
			bit<<=1;
		}
	}
}

HAL_StatusTypeDef wsc1228_start_data_sending()
{
	led_ct=0;
	return HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_1, (uint32_t*)led_dma_buffer, LED_BUF_SIZE);
}

HAL_StatusTypeDef wsc1228_stop_data_sending()
{
	return HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef * htim)
{
	if(led_ct==LED_COUNT)
	{
		wsc1228_stop_data_sending(htim);
	}
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback (TIM_HandleTypeDef * htim)
{

}

void wsc1228_set_color(int c)
{
	color=c;
	fill_dma_buffer();
}

void wsc1228_init_module(TIM_HandleTypeDef* _htim)
{
	htim = _htim;
}
