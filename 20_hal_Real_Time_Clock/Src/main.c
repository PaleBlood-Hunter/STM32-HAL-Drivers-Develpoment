#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "stm32f4xx_hal_uart.h"

#define RTC_ASYNCH_PREDIV		0x7F  //127 decimal
#define RTC_SYNCH_PREDIV		0xF9  //249 decimal

#define BK_FLAG					0x8888

UART_HandleTypeDef huart2;


void uart_init(void);
void rtc_init(void);
void rtc_calendar_config(void);
void rtc_calendar_show(uint8_t *showtime, uint8_t *showdate);

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2,(uint8_t*)&ch,1,10);
	return ch;
}

uint8_t	time[15] = {0};
uint8_t	date[15] = {0};


RTC_HandleTypeDef RTCHandle;

int main()
{

	HAL_Init();
	uart_init();

	__HAL_RTC_RESET_HANDLE_STATE(&RTCHandle);
	rtc_init();

	//READ BACKUP REGISTER
	//if value is different from the value I set, configure calendar
	if(HAL_RTCEx_BKUPRead(&RTCHandle, RTC_BKP_DR0) != BK_FLAG)
	{
		rtc_calendar_config();
	}


	while(1)
	{
		printf('A');
		rtc_calendar_show(time, date);
		HAL_Delay(1000);
	}
}

void rtc_init(void)
{
	//SELECT THE RTC CLOCK
	RCC_OscInitTypeDef				RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef		PeriphClkInitStruct;

	//TURN OFF LSE AND TURN ON LSI
	RCC_OscInitStruct.OscillatorType	=	RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState			=	RCC_LSE_OFF;
	RCC_OscInitStruct.LSIState			=	RCC_LSI_ON;

	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	//SET RTC CLOCK SOURCE TO LSI
	PeriphClkInitStruct.PeriphClockSelection	=	RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection		=	RCC_RTCCLKSOURCE_LSI;

	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	//ENABLE RCC CLOCK
	__HAL_RCC_RTC_ENABLE();

	//CONFIGURE CALENDAR
	RTCHandle.Instance				=	RTC;
	RTCHandle.Init.HourFormat		=	RTC_HOURFORMAT_24;
	RTCHandle.Init.AsynchPrediv		=	RTC_ASYNCH_PREDIV;
	RTCHandle.Init.SynchPrediv		=	RTC_SYNCH_PREDIV;
	RTCHandle.Init.OutPut			=	RTC_OUTPUT_DISABLE;

	HAL_RTC_Init(&RTCHandle);

}

void rtc_calendar_config(void)
{
	RTC_DateTypeDef	sdatestructure;
	RTC_TimeTypeDef stimestructure;

	//SET DATE TO 23/02/2024, FRIDAY
	sdatestructure.Year		=	0x24;
	sdatestructure.Month	=	RTC_MONTH_FEBRUARY;
	sdatestructure.Date		=	0x23;
	sdatestructure.WeekDay	=	RTC_WEEKDAY_FRIDAY;

	HAL_RTC_SetDate(&RTCHandle, &sdatestructure, RTC_FORMAT_BCD);

	//SET TIME TO 14:00:00
	stimestructure.Hours			=	0x14;
	stimestructure.Minutes			=	0x00;
	stimestructure.Seconds			=	0x00;
	stimestructure.TimeFormat		=	RTC_HOURFORMAT12_AM;
	stimestructure.DayLightSaving	=	RTC_DAYLIGHTSAVING_NONE;
	stimestructure.StoreOperation	=	RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&RTCHandle, &stimestructure, RTC_FORMAT_BCD);

	//WRITE TO THE BACKUP REGISTER
	HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, BK_FLAG);
}

void rtc_calendar_show(uint8_t *showtime, uint8_t *showdate)
{
	RTC_DateTypeDef	sdatestructureget;
	RTC_TimeTypeDef stimestructureget;

	HAL_RTC_GetTime(&RTCHandle, &stimestructureget, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&RTCHandle, &sdatestructureget, RTC_FORMAT_BIN);

	//SHOW TIME IN FORMATO HH:MM:SS
	 sprintf((char*)showtime,"%02d:%02d:%02d",stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);

	 //SHOW DATE IN FORMAT MM:DD:YYYY
	 sprintf((char*)showdate,"%02d-%02d-%02d",sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);


	 printf(" Time : %02d:%02d:%02d\n\r",stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
	 printf(" Date : %02d-%02d-%02d\n\r",sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);

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
