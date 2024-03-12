#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

UART_HandleTypeDef huart1;


void uart_init(void);

uint8_t tx_buffer[10] = {10,20,30,40,50,60,70,80,90,100};
uint8_t rx_buffer[10];

char message[18] = "Hello HAL Driver\r\n";


int main()
{

	HAL_Init();
	uart_init();

	HAL_UART_Transmit_IT(&huart1, tx_buffer, 10);
	HAL_UART_Receive_IT(&huart1, rx_buffer, 10);

	while(1)
	{

	}
}



void SysTick_Handler(void)
{
	HAL_IncTick();
}


void uart_init(void)
{

	GPIO_InitTypeDef GPIO_Init_Struct = {0};

	//1. ENABLE CLOCK FOR GPIOA
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. ENABLE CLOCK ON USART2
	__HAL_RCC_USART1_CLK_ENABLE();


	//3. INSTANCING A CLASS OF GPIO INIT TYPE TO CFG ALTERNATE FUNCTION

	GPIO_Init_Struct.Pin 		= GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_Init_Struct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_Init_Struct.Pull 		= GPIO_NOPULL;
	GPIO_Init_Struct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init_Struct.Alternate	= GPIO_AF7_USART1;

	//4. INIT PA2 & PA3 AS USART2
	HAL_GPIO_Init(GPIOA, &GPIO_Init_Struct);

	//CONFIGURE UART MODULE
	huart1.Instance				=	USART1;
	huart1.Init.BaudRate		=	115200;
	huart1.Init.WordLength 		= 	UART_WORDLENGTH_8B;
	huart1.Init.StopBits		=	UART_STOPBITS_1;
	huart1.Init.Parity			=	UART_PARITY_NONE;
	huart1.Init.Mode			=	UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl		=	UART_HWCONTROL_NONE;
	huart1.Init.OverSampling	=	UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart1);

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
}
