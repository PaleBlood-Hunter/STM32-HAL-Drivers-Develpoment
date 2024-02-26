#include "stm32f4xx_hal.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>


#define BTN_PORT		GPIOC
#define BTN_PIN			GPIO_PIN_13

#define	LED_PORT		GPIOA
#define	LED_PIN			GPIO_PIN_5



void gpio_pc13_interrupt_init(void);



int main()
{
	HAL_Init();
	gpio_pc13_interrupt_init();

	while(1)
	{

	}
}



void SysTick_Handler(void)
{
	HAL_IncTick();
}


void gpio_pc13_interrupt_init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();

	//CONFIGURE PC13
	GPIO_InitTypeDef GPIO_InitStruct 	=	 {0};
	GPIO_InitStruct.Pin					=	BTN_PIN;
	GPIO_InitStruct.Mode				=	GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull				=	GPIO_NOPULL;
	GPIO_InitStruct.Speed				=	GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);

	//CONFIGURE PA5
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin					=	LED_PIN;
	GPIO_InitStruct.Mode				=	GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull				=	GPIO_NOPULL;
	GPIO_InitStruct.Speed				=	GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

	//CONFIGURE EXTI
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//DO SOMETHING
	HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
	for(int i = 0; i < 10000; i++);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
