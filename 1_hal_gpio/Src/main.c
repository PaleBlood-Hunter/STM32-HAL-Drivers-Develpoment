#include "stm32f4xx_hal.h"

#define LED_PORT		GPIOA
#define LED_PIN			GPIO_PIN_5

#define BTN_PORT		GPIOC
#define BTN_PIN			GPIO_PIN_13

void pc13_btn_init(void);
void pa5_led_init(void);

uint8_t buttonStatus;
uint8_t ledStatus = 0;

int main()
{
	HAL_Init();

	//1.Configure PA5 as output pin
	pa5_led_init();

	//2.Configure PC13 as input pin
	pc13_btn_init();


	while(1)
	{
		buttonStatus = HAL_GPIO_ReadPin(BTN_PORT, BTN_PIN);

		if(buttonStatus == 0x01)
		{
			ledStatus = 0x00;
		}
		else
		{
			ledStatus = 0x01;
		}

		HAL_GPIO_WritePin(GPIOA, LED_PIN, ledStatus);
	}
}


void pc13_btn_init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE(); //ENABLE CLOCK FOR GPIOC
	GPIO_InitTypeDef	GPIO_InitStruct = {0}; 				//CREATE AN INSTANCE FOR GPIO_InitTypeDef
	GPIO_InitStruct.Pin  	= BTN_PIN;						//SET EACH ONE OF THE ATRIBUTES INSIDE THE STRUCT
	GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed	= GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(BTN_PORT, &GPIO_InitStruct);					//PASS THE INSTANCE FOR THE INIT FUNCTION
}

void pa5_led_init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); 							//ENABLE CLOCK FOR GPIOA
	GPIO_InitTypeDef	GPIO_InitStruct = {0}; 				//CREATE AN INSTANCE FOR GPIO_InitTypeDef
	GPIO_InitStruct.Pin  	= LED_PIN;						//SET EACH ONE OF THE ATRIBUTES INSIDE THE STRUCT
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed	= GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);					//PASS THE INSTANCE FOR THE INIT FUNCTION
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}
