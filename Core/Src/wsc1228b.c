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
uint16_t ccr_ret = 0;

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
	memset(led_dma_buffer,0, LED_BUF_SIZE*sizeof(uint16_t));

	for(int rgb_idx=0; rgb_idx<3; rgb_idx++)
	{
		bit=1 << 7;
		for(int bit_idx=0; bit_idx<8; bit_idx++)
		{
			if( colors[color][rgb_idx] & bit )
			{
				led_dma_buffer[rgb_idx*8+bit_idx]=ccr_1;

			}
			else
			{
				led_dma_buffer[rgb_idx*8+bit_idx]=ccr_0;
			}
			bit >>= 1;
		}
	}
}

void transferCmplt()
{
	led_ct++;
	//LOG("transferCmplt - LED_COUNT=%d, led_ct=%d\r\n",LED_COUNT, led_ct);
	if(led_ct==LED_COUNT)
	{
		HAL_StatusTypeDef status=wsc1228_stop_data_sending(htim);
		//*
		if(status==HAL_OK)
		{
			LOG("stop_data_sending OK.  LED_COUNT=%d, led_ct=%d \r\n",LED_COUNT, led_ct);
		}
		else
		{
			LOG("stop_data_sending ERROR - %d\r\n", status);
		}
		//*/
	}
}

void transferHalfCmplt()
{

}

void transferError()
{

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
	     led_ct++;
		//LOG("transferCmplt - LED_COUNT=%d, led_ct=%d\r\n",LED_COUNT, led_ct);
		if(led_ct==LED_COUNT)
		{
			HAL_StatusTypeDef status=wsc1228_stop_data_sending(htim);
			//*
			if(status==HAL_OK)
			{
				LOG("stop_data_sending OK.  LED_COUNT=%d, led_ct=%d \r\n",LED_COUNT, led_ct);
			}
			else
			{
				LOG("stop_data_sending ERROR - %d\r\n", status);
			}
			//*/
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
