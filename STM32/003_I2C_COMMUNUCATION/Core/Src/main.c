/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEMP_OUT_INT_REGISTER   0x00
#define TEMP_OUT_FRAC_REGISTER  0x01
#define WHO_AM_I_REGISTER       0x0F
#define WHO_AM_I_VALUE          0xBC
#define TRANSFER_DIR_WRITE      0x01
#define TRANSFER_DIR_READ       0x00
#define I2C_SLAVE_ADDR          0x07
#define SLAVE_BOARD
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint8_t transferDirection, transferRequested;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

#ifdef SLAVE_BOARD
static void MX_ADC1_Init(void);
#endif
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char uartBuf[25];
	uint8_t i2cBuf[2] = {0};
	float ftemp,ftemp_temporary;
	volatile int8_t t_frac = 0, t_int= 0;
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
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
#ifdef SLAVE_BOARD
  uint16_t rawValue;
  uint32_t lastConversion;

  MX_ADC1_Init();
  HAL_ADC_Start(&hadc1);
  while(1) {
    HAL_I2C_EnableListen_IT(&hi2c1);
    while(!transferRequested) {
      if(HAL_GetTick() - lastConversion > 1000L) {
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

        rawValue = HAL_ADC_GetValue(&hadc1);
        ftemp = ((float)rawValue) / 4095 * 3000;
        ftemp = ((ftemp - 760.0) / 2.5) + 25;

        t_int = ftemp;
        t_frac = (ftemp - t_int)*100;

        sprintf(uartBuf, "Temperature: %.2f\r\n", ftemp);
        HAL_UART_Transmit(&huart2, (uint8_t*)uartBuf, strlen(uartBuf), HAL_MAX_DELAY);

        sprintf(uartBuf, "t_int: %d - t_frac: %d\r\n", t_int, t_frac);
        HAL_UART_Transmit(&huart2, (uint8_t*)uartBuf, strlen(uartBuf), HAL_MAX_DELAY);

        lastConversion = HAL_GetTick();
      }
    }

    transferRequested = 0;

    if(transferDirection == TRANSFER_DIR_WRITE) {
      /* Master is sending register address */
      HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, i2cBuf, 1, I2C_FIRST_FRAME);
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_LISTEN);

      switch(i2cBuf[0]) {
        case WHO_AM_I_REGISTER:
          i2cBuf[0] = WHO_AM_I_VALUE;
          break;
        case TEMP_OUT_INT_REGISTER:
          i2cBuf[0] = t_int;
          break;
        case TEMP_OUT_FRAC_REGISTER:
          i2cBuf[0] = t_frac;
          break;
        default:
          i2cBuf[0] = 0xFF;
      }

      HAL_I2C_Slave_Sequential_Transmit_IT(&hi2c1, i2cBuf, 1, I2C_LAST_FRAME);
      while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    }
  }

#else //Master board
while(1){
	  i2cBuf[0] = WHO_AM_I_REGISTER;
	  	HAL_I2C_Master_Seq_Transmit_IT(&hi2c1, I2C_SLAVE_ADDR, i2cBuf,
	                                          1, I2C_FIRST_FRAME);
	    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	    HAL_I2C_Master_Seq_Receive_IT(&hi2c1, I2C_SLAVE_ADDR, i2cBuf,
	                                         1, I2C_LAST_FRAME);
	    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	    sprintf(uartBuf, "WHO AM I: %x\r\n", i2cBuf[0]);
	    HAL_UART_Transmit(&huart2, (uint8_t*) uartBuf, 1, HAL_MAX_DELAY);

	    i2cBuf[0] = TEMP_OUT_INT_REGISTER;
	    HAL_I2C_Master_Seq_Transmit_IT(&hi2c1, I2C_SLAVE_ADDR, i2cBuf,
	                                          1, I2C_FIRST_FRAME);
	    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	    HAL_I2C_Master_Seq_Receive_IT(&hi2c1, I2C_SLAVE_ADDR, (uint8_t*)&t_int,
	                                         1, I2C_LAST_FRAME);
	    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	    i2cBuf[0] = TEMP_OUT_FRAC_REGISTER;
	    HAL_I2C_Master_Seq_Transmit_IT(&hi2c1, I2C_SLAVE_ADDR, i2cBuf,
	                                          1, I2C_FIRST_FRAME);
	    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);

	    HAL_I2C_Master_Seq_Receive_IT(&hi2c1, I2C_SLAVE_ADDR, (uint8_t*)&t_frac,
	                                         1, I2C_LAST_FRAME);
	    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
/*
	    ftemp = t_int;
	    ftemp_temporary = t_frac;
	    ftemp_temporary = (ftemp_temporary)/100;
	    ftemp += ftemp_temporary;
*/
	    ftemp = ((float)t_frac)/100.0;
	      ftemp += (float)t_int;

	    /*sprintf(uartBuf, "t_int: %d\r\n", (uint8_t)t_int);
	    	    HAL_UART_Transmit(&huart2, (uint8_t*) uartBuf, 1, HAL_MAX_DELAY);
	    	    HAL_Delay(100);
	    	    sprintf(uartBuf, "t_frac: %d\r\n", (uint8_t)t_frac);
	    	    	    HAL_UART_Transmit(&huart2, (uint8_t*) uartBuf, 1, HAL_MAX_DELAY);
	    	    	    HAL_Delay(100);*/
	    sprintf(uartBuf, "Temperature: %.2f\r\n", ftemp);
	    HAL_UART_Transmit(&huart2, (uint8_t*) uartBuf, strlen(uartBuf), HAL_MAX_DELAY);
	    HAL_Delay(1000);
}
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = I2C_SLAVE_ADDR;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  huart2.Init.BaudRate = 38400;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
#ifdef SLAVE_BOARD
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  UNUSED(AddrMatchCode);

  if(hi2c->Instance == I2C1) {
    transferRequested = 1;
    transferDirection = TransferDirection;
  }
}
#endif
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

