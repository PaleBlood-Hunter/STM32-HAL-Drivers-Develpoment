#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "uart.h"
#include "adc.h"

#define BUFFER_SIZE				32
#define DMA_STREAM				DMA2_Stream0
#define DMA_STREAM_IRQ			DMA2_Stream0_IRQn
#define DMA_STREAM_IRQHANDLER	DMA2_Stream0_IRQHandler
#define DMA_CHANNEL				DMA_CHANNEL_0

DMA_HandleTypeDef				DmaHandle;

static const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};

static uint32_t aDST_Buffer[BUFFER_SIZE];

static void TransferComplete(DMA_HandleTypeDef *DmaHandle);
static void TransferError(DMA_HandleTypeDef *DmaHandle);
static void dma_config(void);



int main()
{

	HAL_Init();
	uart_init();

	dma_config();

	while(1)
	{

	}
}

static void dma_config(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();

	DmaHandle.Instance					= 	DMA_STREAM;
	DmaHandle.Init.Channel  			= 	DMA_CHANNEL;
	DmaHandle.Init.Direction			=	DMA_MEMORY_TO_MEMORY;
	DmaHandle.Init.PeriphDataAlignment	=	DMA_PDATAALIGN_WORD; //because data is 32 bits
	DmaHandle.Init.MemDataAlignment		=	DMA_MDATAALIGN_WORD; //because data is 32 bits
	DmaHandle.Init.PeriphInc			=	DMA_PINC_ENABLE;
	DmaHandle.Init.MemInc				=	DMA_MINC_ENABLE;
	DmaHandle.Init.Mode					=	DMA_NORMAL;
	DmaHandle.Init.Priority				=	DMA_PRIORITY_HIGH;
	DmaHandle.Init.FIFOMode				=	DMA_FIFOMODE_ENABLE;
	DmaHandle.Init.FIFOThreshold		=	DMA_FIFO_THRESHOLD_1QUARTERFULL;
	DmaHandle.Init.MemBurst				=	DMA_MBURST_SINGLE;

	HAL_DMA_Init(&DmaHandle);

	//Register callback functions
	HAL_DMA_RegisterCallback(&DmaHandle, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
	HAL_DMA_RegisterCallback(&DmaHandle, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

	HAL_NVIC_SetPriority(DMA_STREAM_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(DMA_STREAM_IRQ);

	HAL_DMA_Start_IT(&DmaHandle, (uint32_t)&aSRC_Const_Buffer, (uint32_t)&aDST_Buffer, BUFFER_SIZE);
}

static void TransferComplete(DMA_HandleTypeDef *DmaHandle)
{
	//Do somthing..
	printf("Transfer complete!! \n\r");
}


static void TransferError(DMA_HandleTypeDef *DmaHandle)
{
	//Do somthing...
	printf("Transfer Error!! \n\r");
}


void DMA_STREAM_IRQHANDLER(void)
{
    HAL_DMA_IRQHandler(&DmaHandle);
}


void SysTick_Handler(void)
{
	HAL_IncTick();
}


