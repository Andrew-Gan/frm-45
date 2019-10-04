/*
 * MyTimers.c
 *
 *  Created on: 28.04.2017
 *      Author: NELO
 */

#include "stm32l4xx.h"
#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_tim.h"
#include "MyTimers_HAL.h"
#include "stm32l4xx_hal_rcc.h"


TIM_HandleTypeDef htim15;

/* TIM15 Init function , delays*/
void InitandEnableDelayTimer15(void)
{
	__HAL_RCC_TIM15_CLK_ENABLE();			//// Enable Timer Clock
	htim15.Instance = TIM15;
	htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	htim15.Init.Prescaler = (HAL_RCC_GetHCLKFreq()/1000000);	// RCC_Clocks/1000 for 1ms /1000000 for 1us, !MAx value 0xFFFF = 65535 !;
	htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim15.Init.Period = 65535;
	htim15.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&htim15);
}

////us delays
void delay_us(uint16_t delay_time){
	__HAL_TIM_SET_COUNTER(&htim15,0);
	HAL_TIM_Base_Start(&htim15);
	while(__HAL_TIM_GET_COUNTER(&htim15) != delay_time);				// wait
}

////ms delays
void delay_ms(uint32_t delay_time){
	if(delay_time > 0 ){
		do{
			delay_us(1000);
		}while(--delay_time);
	}
}

////s delays
void delay_s(uint32_t delay_time){
	if(delay_time > 0 ){
		do{
			delay_ms(1000);
		}while(--delay_time);
	}
}
