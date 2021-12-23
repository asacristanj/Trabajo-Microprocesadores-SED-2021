/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
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
#include "stdlib.h"
#include "string.h"
#include "TFT.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//Interrupciones botones:
volatile int button[10];
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	void rellenar_botones(int n) {
		for (int i = 0; i < 10; i++) {
			if (i != n)
				button[i] = 0;
		}
		button[n] = 1;
	}

	if (GPIO_Pin == GPIO_PIN_0) {
		rellenar_botones(0);
	} else if (GPIO_Pin == GPIO_PIN_1) {
		rellenar_botones(1);
	}

}
//Antirrebotes botones:
int debouncer(volatile int *button_int, GPIO_TypeDef *GPIO_port,
		uint16_t GPIO_number) {
	static uint8_t button_count = 0;
	static int counter = 0;

	if (*button_int == 1) {
		if (button_count == 0) {
			counter = HAL_GetTick();
			button_count++;
		}
		if (HAL_GetTick() - counter >= 20) {
			counter = HAL_GetTick();
			if (HAL_GPIO_ReadPin(GPIO_port, GPIO_number) != 1) {
				button_count = 1;
			} else {
				button_count++;
			}
			if (button_count == 4) { //Periodo antirebotes
				button_count = 0;
				*button_int = 0;
				return 1;
			}
		}
	}
	return 0;
}
/*
//--------------------------------7 SEGMENTOS------------------------//
//Mostrar texto en pantalla 7 segmentos
void mostrartexto7s(const char palabra[]) {
	uint8_t longitud = strlen(palabra);
	uint8_t texto[longitud + 1];
	texto[0] = 1;
	for (int i = 1; i < longitud + 1; i++) {
		texto[i] = palabra[i - 1];
	}
	HAL_I2C_Master_Transmit(&hi2c1, 0x7 << 1, texto, longitud + 1, 30);
}
//Mostrar numero en pantalla 7 segmentos
void mostrarnum7s(float num) {
	uint8_t cadena_num[5];
	cadena_num[0] = 0;
	if (num < 10) {
		cadena_num[1] = 2;
		num = num * 100;
	} else if (num < 100) {
		cadena_num[1] = 1;
		num = num * 10;
	} else {
		cadena_num[1] = 0;
	}
	cadena_num[2] = (num / 100);
	cadena_num[3] = num / 10 - cadena_num[2] * 10;
	cadena_num[4] = (num - cadena_num[2] * 100 - cadena_num[3] * 10);
	HAL_I2C_Master_Transmit(&hi2c1, 0x7 << 1, cadena_num, 5, 30);
}
*/

//botones b_luces(30, 70, 150, 140, 2, 5, {0, 0, 255}, {255, 255, 255}, {255, 0, 0}, {255, 255, 255}, 1, 2 );
const char *t_menu[6] = { "LUCES", "PERSIANAS", "RIEGO", "CLIMA", "AUDIO",
		"SEGURIDAD" };
//const char* t_luces[2] = {"AUTOMATICO", "MANUAL"};
char rx_buffer[50], tx_buffer[50];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	int opcion = 0;
	pantalla_principal();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_UART_Receive(&huart6, (uint8_t*)rx_buffer, 50, 500);
		if (rx_buffer[0]=='r') {
			if (opcion == 3) {
				opcion = 1;
			}else if (opcion == 6) {
				opcion = 4;
			}else{
				opcion++;
			}
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			resaltar_boton(opcion);
			setFont(1);
			rellenar_botones(t_menu);
			rx_buffer[0]=0;
		} else if (rx_buffer[0]=='l') {
			if (opcion == 1) {
				opcion = 3;
			}else if (opcion == 4) {
				opcion = 6;
			}else{
				opcion--;
			}
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			resaltar_boton(opcion);
			setFont(1);
			rellenar_botones(t_menu);
			rx_buffer[0]=0;
		}
		else if (rx_buffer[0]=='u') {
					if (opcion == 1) {
						opcion = 4;
					}else if (opcion == 2) {
						opcion = 5;
					}else if (opcion == 3) {
						opcion = 6;
					}else{
						opcion-=3;
					}
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
					resaltar_boton(opcion);
					setFont(1);
					rellenar_botones(t_menu);
					rx_buffer[0]=0;
				}
		else if (rx_buffer[0]=='d') {
					if (opcion == 4) {
						opcion = 1;
					}else if (opcion == 5) {
						opcion = 2;
					}else if (opcion == 6) {
						opcion = 3;
					}else{
						opcion+=3;
					}
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
					resaltar_boton(opcion);
					setFont(1);
					rellenar_botones(t_menu);
					rx_buffer[0]=0;
				}

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  hi2c1.Init.OwnAddress1 = 0;
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
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

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
	while (1) {
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

