#include "stm32f4xx_hal.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>


ADC_HandleTypeDef hadc1;

char message[18] = "Hello HAL Driver\r\n";
uint32_t sensor_value[1];

int main()
{

	HAL_Init();
	uart_init();
	adc_pa0_dma_init();

	//1. START ADC
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_value, 1);


	while(1)
	{

		printf("The sensor value is: %d  \n\r", (int)sensor_value);
		//HAL_Delay(1000);
	}
}


//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//	//do something
//	//2. GET CONVERSION
//	sensor_value = pa0_adc_read();
//}


void SysTick_Handler(void)
{
	HAL_IncTick();
}


