/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx_hal.h"
			

void SystemClock_Config(void);

void LEDConfig(void);
void RTCConfig(void);

	GPIO_InitTypeDef led_init;
	RTC_HandleTypeDef my_RTC;
	RTC_AlarmTypeDef my_alarm;

	RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

int main(void){
	HAL_Init();
	SystemClock_Config();
	LEDConfig();
	RTCConfig();

	//Set time, data and alarm
	//1) Set time
	sTime.Hours = 23;
	sTime.Minutes = 59;
	sTime.Seconds = 45;
	HAL_RTC_SetTime(&my_RTC, &sTime, RTC_FORMAT_BIN);
	//2) Set date
	sDate.Date = 31;
	sDate.Month = RTC_MONTH_DECEMBER;
	sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	sDate.Year = 17;
	HAL_RTC_SetDate(&my_RTC, &sDate, RTC_FORMAT_BIN);



	while(1)
	{
		HAL_RTC_GetTime(&my_RTC, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&my_RTC, &sDate, RTC_FORMAT_BIN);
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
		HAL_Delay(400);
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
	}
}




void LEDConfig(void){
	__HAL_RCC_GPIOG_CLK_ENABLE();

	led_init.Pin = GPIO_PIN_13 | GPIO_PIN_14;
		led_init.Mode = GPIO_MODE_OUTPUT_PP;
	led_init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOG, &led_init);

}

void RTCConfig(void){
	__HAL_RCC_RTC_ENABLE();
	my_RTC.Instance = RTC;
	my_RTC.Init.HourFormat = RTC_HOURFORMAT_24;
	my_RTC.Init.SynchPrediv = 256-1;
	my_RTC.Init.AsynchPrediv = 128-1;
	my_RTC.Init.OutPut = RTC_OUTPUT_DISABLE;
	HAL_RTC_Init(&my_RTC);

	//konfiguracja alarmu
	my_alarm.Alarm = RTC_ALARM_A;
	my_alarm.AlarmTime.Hours = 0;
	my_alarm.AlarmTime.Minutes = 0;
	my_alarm.AlarmTime.Seconds = 10;
	my_alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE; //podanie dnia miesiaca; druga opcja to dzien tygodnia (nazwa)
	my_alarm.AlarmDateWeekDay = 1;
	HAL_RTC_SetAlarm_IT(&my_RTC, &my_alarm, RTC_FORMAT_BIN);
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
   // Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    //Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
  //  Error_Handler();
  }
}
void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&my_RTC);
}
