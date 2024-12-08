/*
 * nec.c
 *
 *  Created on: Nov 14, 2024
 *      Author: David Jashi
 */

#include "nec.h"

#define NEC_0 500 //1125
#define NEC_1 1000 //2250

uint8_t command[4];

static TIM_HandleTypeDef* htim;

static uint8_t receiving_signal = 0;
static uint32_t bit_counter = 0;
static uint32_t data=0;

uint16_t width_vals[32];
uint16_t ct_values[32];

volatile uint8_t  SYM=0;
volatile uint8_t  SYM_EVENT=0;

struct IR_CODE
{
	uint32_t code;
	uint8_t  sym;
};

const struct IR_CODE ir_codes[10]={
		{0x6916ff00, 0},
		{0x730cff00, 1},
		{0x6718ff00, 2},
		{0x215eff00, 3},
		{0x7708ff00, 4},
		{0x631cff00, 5},
		{0x255aff00, 6},
		{0x3d42ff00, 7},
		{0x2d52ff00, 8},
		{0x354aff00, 9}
};


uint8_t getsym(uint32_t ir_code)
{
	uint8_t result=1;
	for(int i=0; i<10; i++)
	{
		if(ir_codes[i].code==ir_code)
		{
			result = ir_codes[i].sym;
			break;
		}
	}
	return result;
}

void process_data()
{
//	char* dp=(char*)&data;
//	for(int i=0; i<4; i++)
//	{
//		command[i]=*(dp+i);
//	}

	//if( (command[0]==~command[1]) && (command[2]==~command[3]) )


	uint32_t bit = 1;
	int k = bit_counter <= 32 ? bit_counter : 32;
	for(int i=0; i<k; i++)
	{
		//LOG("%d ==>  %d\r\n", i, ct_values[i]);
		if(i > 0)
		{
			uint16_t pulse_width=ct_values[i]-ct_values[i-1];
			//LOG("bit=0x%04x ==>  pulse_width=%d\r\n", i,pulse_width);
			if( pulse_width > NEC_1 )
			{
				data=data|bit;
			}
			bit=bit<<1;
		}
	}

	//LOG("\r\n");
	//LOG("********************************\r\n");
	if(data!=0)
	{
		SYM=getsym(data);
		LOG("data=0x%04x   sym=%d\r\n",data,SYM);
		//change_color(SYM);
		SYM_EVENT=1;
	}
		//LOG("addr=%d 0x%02x, command=%d %02x\r\n",command[0],command[0],command[2],command[2]);
	//LOG("addr_inv=0x%02x, command_inv=%02x\r\n",command[1],command[1],command[3],command[3]);
	//LOG("********************************\r\n");


	bit_counter=0;
	data=0;
	receiving_signal=0;


}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance==TIM2)
	{
		//HAL_TIM_Base_Stop(htim);
		//HAL_GPIO_WritePin(DEGUG_LED_GPIO_Port, DEGUG_LED_Pin,1);
		if(receiving_signal)
		{
			process_data();
		}
		//HAL_Delay(20);
		//HAL_GPIO_WritePin(DEGUG_LED_GPIO_Port, DEGUG_LED_Pin,0);
		//LOG("=== TIMER_INTERRUPT  HAL_TIM_PeriodElapsedCallback===\r\n");
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	    if(!receiving_signal)
	    {
	    	 __HAL_TIM_SET_COUNTER(htim, 0);
	    	   receiving_signal=1;
	    }
	    else
	    {
	    	uint16_t t=__HAL_TIM_GET_COUNTER(htim);
	    	ct_values[bit_counter]=t;
	    	//LOG("%d >> %d\r\n", bit_counter, t);
	    	bit_counter++;
	    }
}


void nec_init(TIM_HandleTypeDef* _htim)
{
	htim = _htim;
	HAL_TIM_Base_Start_IT(htim);
	LOG("=== TIME 2 STARTED ===\r\n");
}
