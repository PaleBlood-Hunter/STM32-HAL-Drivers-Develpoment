#include "stm32f4xx_hal.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>

SPI_HandleTypeDef	hspi1;

void spi1_init(void);

uint8_t	tx_buffer[10] = {10,20,20,40,50,60,70,90,90,100};
uint8_t	rx_buffer[10];

int main()
{

	HAL_Init();
	spi1_init();

	HAL_SPI_TransmitReceive(&hspi1, tx_buffer, rx_buffer, 10, 100);

	while(1)
	{

	}
}

void spi1_init(void)
{
	//ENACLO CLOCK TO SPI1
	__HAL_RCC_SPI1_CLK_ENABLE();
	//ENABLE CLOCK ACCESS TO GPIO A (SCK = PA5, MISO = PA6, MOSI = PA7)
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin			=	GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode		=	GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate	=	GPIO_AF5_SPI1;
	GPIO_InitStruct.Pull		=	GPIO_NOPULL;
	GPIO_InitStruct.Speed		=	GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//CONFIGURE THE SPI MODULE
	hspi1.Instance						=		SPI1;
	hspi1.Init.Mode						=		SPI_MODE_MASTER;
	hspi1.Init.Direction				=		SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize					=		SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity				=		SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase					=		SPI_PHASE_1EDGE;
	hspi1.Init.NSS						=		SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler		=		SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit					=		SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode					=		SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation			=		SPI_CRCCALCULATION_DISABLE;

	HAL_SPI_Init(&hspi1);

}


void SysTick_Handler(void)
{
	HAL_IncTick();
}


