#include "stm32f4xx_hal.h"

UART_HandleTypeDef huart2;

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 10);
	return ch;
}

void uart_init(void)
{

	GPIO_InitTypeDef GPIO_Init_Struct = {0};

	//1. ENABLE CLOCK FOR GPIOA
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. ENABLE CLOCK ON USART2
	__HAL_RCC_USART2_CLK_ENABLE();


	//3. INSTANCING A CLASS OF GPIO INIT TYPE TO CFG ALTERNATE FUNCTION

	GPIO_Init_Struct.Pin 		= GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_Init_Struct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_Init_Struct.Pull 		= GPIO_NOPULL;
	GPIO_Init_Struct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init_Struct.Alternate	= GPIO_AF7_USART2;

	//4. INIT PA2 & PA3 AS USART2
	HAL_GPIO_Init(GPIOA, &GPIO_Init_Struct);

	//CONFIGURE UART MODULE
	huart2.Instance				=	USART2;
	huart2.Init.BaudRate		=	115200;
	huart2.Init.WordLength 		= 	UART_WORDLENGTH_8B;
	huart2.Init.StopBits		=	UART_STOPBITS_1;
	huart2.Init.Parity			=	UART_PARITY_NONE;
	huart2.Init.Mode			=	UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl		=	UART_HWCONTROL_NONE;
	huart2.Init.OverSampling	=	UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart2);

}
