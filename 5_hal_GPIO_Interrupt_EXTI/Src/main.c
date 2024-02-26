#include "stm32f4xx_hal.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>


char message[18] = "Hello HAL Driver\r\n";
uint32_t sensor_value;

int main()
{

	HAL_Init();
	uart_init();

	adc_pa0_single_conv_init();


	while(1)
	{
		//1. start adc

		//2. Poll for Conversion

		//3. Get Conversion


		sensor_value = pa0_adc_read();

		printf("Printf is being used! \n\r");
		//HAL_Delay(1000);
	}
}



void SysTick_Handler(void)
{
	HAL_IncTick();
}


