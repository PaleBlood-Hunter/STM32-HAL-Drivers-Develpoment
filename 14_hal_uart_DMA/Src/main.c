#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef  hdma_usart1_rx;
DMA_HandleTypeDef  hdma_usart1_tx;


void uart_init(void);

uint8_t tx_buffer[10] = {10,20,30,40,50,60,70,80,90,100};
uint8_t rx_buffer[10];

char message[18] = "Hello HAL Driver\r\n";

uint32_t rx_counter, tx_counter;


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	tx_counter++;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	rx_counter++;
}

int main()
{

	HAL_Init();
	uart_init();

	HAL_UART_Transmit_DMA(&huart1, tx_buffer, 10);
	HAL_UART_Receive_DMA(&huart1, rx_buffer, 10);

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

	// ENABLE CLOCK FOR THE DMA
	__HAL_RCC_DMA2_CLK_ENABLE();


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

	//Configure USART1 RX DMA
	hdma_usart1_rx.Instance					 = 	DMA2_Stream2; //IN THE STREAM AND CHANNEL DMA TABLE
	hdma_usart1_rx.Init.Channel				 =	DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction			 =	DMA_PERIPH_TO_MEMORY;//BECAUSE I AM RECEIVING DATA
	hdma_usart1_rx.Init.PeriphInc			 =	DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc				 =	DMA_MINC_ENABLE; //WHEN STORE DATA IN ARRAY WE NEED TO INCREMENT
	hdma_usart1_rx.Init.PeriphDataAlignment  =  DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment	 =	DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode				 =	DMA_NORMAL;
	hdma_usart1_rx.Init.Priority			 =	DMA_PRIORITY_LOW;
	hdma_usart1_rx.Init.FIFOMode			 =	DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma_usart1_rx);

	//Link usart1 rx to dma
	huart1.hdmarx = &hdma_usart1_rx;

	//CONFIGURE USART1 TX DMA
	hdma_usart1_tx.Instance					 = 	DMA2_Stream7; //IN THE STREAM AND CHANNEL DMA TABLE
	hdma_usart1_tx.Init.Channel				 =	DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction			 =	DMA_MEMORY_TO_PERIPH; //BECAUSE I AM SENDING DATA
	hdma_usart1_tx.Init.PeriphInc			 =	DMA_PINC_DISABLE;
	hdma_usart1_tx.Init.MemInc				 =	DMA_MINC_ENABLE; //WHEN STORE DATA IN ARRAY WE NEED TO INCREMENT
	hdma_usart1_tx.Init.PeriphDataAlignment  =  DMA_PDATAALIGN_BYTE;
	hdma_usart1_tx.Init.MemDataAlignment	 =	DMA_MDATAALIGN_BYTE;
	hdma_usart1_tx.Init.Mode				 =	DMA_NORMAL;
	hdma_usart1_tx.Init.Priority			 =	DMA_PRIORITY_LOW;
	hdma_usart1_tx.Init.FIFOMode			 =	DMA_FIFOMODE_DISABLE;


	HAL_DMA_Init(&hdma_usart1_tx);

	//Link usart1 tx to dma
	huart1.hdmatx = &hdma_usart1_tx;

	//DMA2 Stream2_IRQn INTERRUPT CONFIGURATION
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	//DMA2 Stream7_IRQn INTERRUPT CONFIGURATION
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

void DMA2_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
}
