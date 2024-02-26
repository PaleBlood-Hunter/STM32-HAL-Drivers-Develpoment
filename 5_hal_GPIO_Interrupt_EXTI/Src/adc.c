/*
 * adc.c
 *
 *  Created on: Feb 5, 2024
 *      Author: vinicius.ferreira
 */

#include "stm32f4xx_hal.h"

ADC_HandleTypeDef hadc1;

static void adc_pa0_continous_conv_init(void);

uint32_t pa0_adc_read(void)
{
	return HAL_ADC_GetValue(&hadc1);
}
void adc_start(void)
{
	adc_pa0_continous_conv_init();
	HAL_ADC_Start(&hadc1);
}
void adc_pa0_continous_conv_init(void)
{
	//configure pa0 as analog pin
	__HAL_RCC_GPIOA_CLK_ENABLE(); //ENABLE CLOCK FOR GPIOC
	GPIO_InitTypeDef	GPIO_InitStruct = {0}; 	//CREATE AN INSTANCE FOR GPIO_InitTypeDef
	ADC_ChannelConfTypeDef sConfig 		= {0};

	GPIO_InitStruct.Pin  	= GPIO_PIN_0;						//SET EACH ONE OF THE ATRIBUTES INSIDE THE STRUCT
	GPIO_InitStruct.Mode 	= GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	//PASS THE INSTANCE FOR THE INIT FUNCTION


	//configure adc module for continous conversion
	__HAL_RCC_ADC1_CLK_ENABLE();
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler			=		ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution				=		ADC_RESOLUTION_12B;
	hadc1.Init.ContinuousConvMode		=	ENABLE;
	hadc1.Init.DiscontinuousConvMode	=	DISABLE;
	hadc1.Init.ExternalTrigConvEdge		=	ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv			=	ADC_SOFTWARE_START;
	hadc1.Init.DataAlign				=	ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion			=	1;
	hadc1.Init.DMAContinuousRequests	=	DISABLE;
	hadc1.Init.EOCSelection				=	ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	sConfig.Channel			=	ADC_CHANNEL_0;
	sConfig.Rank			=	1;
	sConfig.SamplingTime	=	ADC_SAMPLETIME_480CYCLES;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}
