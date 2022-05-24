/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
ADC_ChannelConfTypeDef sConfig = {0};	// Use this in ADC Init
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

ADC_ChannelConfTypeDef MX_ADC1_Init(ADC_ChannelConfTypeDef sConfig);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float ADC1_IN_Temp[12];
char bufferInitialText[100] = "STAMP ADC1_IN0 ADC1_IN1 ADC1_IN4 ADC1_IN5 ADC1_IN6 ADC1_IN7 ADC1_IN8 ADC1_IN9 ADC1_IN10 ADC1_IN11 \n";
char bufferToUART[200];										// This buffer is sent via UART
uint8_t charCountUARTbuffer = 0;							// To count characters in UART going buffer
uint32_t counter;
HAL_StatusTypeDef statusCheckUART = {2};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	float milliVoltsPerValue = 0.805860;
	int sampleQuantity = 500;
	float ADC1_IN[sampleQuantity][12];	// TODO Remember to use this
	float ADC1_IN_mv[12];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  sConfig = MX_ADC1_Init(sConfig);			// Use this in ADC Init to return the sConfig from channel settings
  statusCheckUART = HAL_UART_Transmit(&huart2, (uint8_t*)bufferInitialText, sizeof(bufferInitialText), 500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  for(int i = 0; i < sampleQuantity; i++)
	  {
		  // TODO Implement averaging
		  // TODO Implement cold junction compensation
		  sConfig.Channel = ADC_CHANNEL_0;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][0] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_1;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][1] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_4;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][4] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_5;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][5] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_6;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][6] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_7;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][7] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_8;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][8] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_9;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][9] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_10;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][10] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);

		  sConfig.Channel = ADC_CHANNEL_11;
		  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 500);
		  ADC1_IN[i][11] = HAL_ADC_GetValue(&hadc1);
		  //HAL_Delay(100);
		  HAL_ADC_Stop(&hadc1);
	  }
	  for(int i = 0; i < 12; i++)
	  {
		  float tempSum = 0;
		  for(int j = 0; j < sampleQuantity; j++)
		  {
			  tempSum = tempSum + ADC1_IN[j][i];
		  }
		  ADC1_IN_mv[i] = tempSum / sampleQuantity;
		  ADC1_IN_mv[i] = ADC1_IN_mv[i] * milliVoltsPerValue;
		  tempSum = 0;
	  }

	  ADC1_IN_Temp[0] = (ADC1_IN_mv[0] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[1] = (ADC1_IN_mv[1] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[4] = (ADC1_IN_mv[4] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[5] = (ADC1_IN_mv[5] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[6] = (ADC1_IN_mv[6] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[7] = (ADC1_IN_mv[7] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[8] = (ADC1_IN_mv[8] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[9] = (ADC1_IN_mv[9] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[10] = (ADC1_IN_mv[10] / 2.2211) - 133.33 - 1.8 + 5.3;
	  ADC1_IN_Temp[11] = (ADC1_IN_mv[11] / 2.2211) - 133.33 - 1.8 + 5.3;

	  charCountUARTbuffer = snprintf(bufferToUART, sizeof(bufferToUART), "Stamp %d A0 %.1f A1 %.1f A4 %.1f A5 %.1f A6 %.1f A7 %.1f A8 %.1f A9 %.1f A10 %.1f A11 %.1f \n \r",
			  counter, ADC1_IN_Temp[0], ADC1_IN_Temp[1], ADC1_IN_Temp[4], ADC1_IN_Temp[5], ADC1_IN_Temp[6], ADC1_IN_Temp[7], ADC1_IN_Temp[8], ADC1_IN_Temp[9], ADC1_IN_Temp[10], ADC1_IN_Temp[11]);

	  statusCheckUART = HAL_UART_Transmit(&huart2, (uint8_t*)bufferToUART, sizeof(bufferToUART), 500);

	  //HAL_Delay(974); 	// TODO Remove this later
	  counter++;		// TODO Better implementation
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
ADC_ChannelConfTypeDef MX_ADC1_Init(ADC_ChannelConfTypeDef sConfig)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  //ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 10;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 9;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 10;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  return sConfig;

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
