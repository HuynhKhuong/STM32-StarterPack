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
#include "led.h"
#include "package_handle.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum{
	ID_GET = 0,
	ID_SET,
	LED_GET,
	LED_SET,
}func_enumerate;

typedef struct{
	struct Package_Typedef* COMprocess;
	char SystemLED[2];
	char ID[6];
}System_Typedef;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
struct LED* myLED;
System_Typedef* my_System;
void (*func_enum[5])(System_Typedef*);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
System_Typedef* System_Ctor(void);
void ID_update(System_Typedef* me);
void ID_feedback(System_Typedef* me);
void LED_update(System_Typedef* me);
void LED_feedback(System_Typedef* me);
void doing_nothing(System_Typedef* me);
packageState_Typedef _LEDset(struct Package_Typedef *me);

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	//my_Package = Package_Ctor();
	myLED = Led_ctor();
	my_System = System_Ctor();
	func_enum[0] = ID_feedback;
	func_enum[1] = ID_update;
	func_enum[2] = LED_feedback;
	func_enum[3] = LED_update;
	func_enum[4] = doing_nothing;
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)(my_System->COMprocess->receive_package), 50);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(my_System->COMprocess->mode < 4) {
			func_enum[my_System->COMprocess->mode](my_System);
			Package_seal(my_System->COMprocess);
			HAL_UART_Transmit_DMA(&huart1, (uint8_t*)my_System->COMprocess->response_package, strlen(my_System->COMprocess->response_package));
			my_System->COMprocess->mode = 4;
		}

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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
packageState_Typedef _LEDset(struct Package_Typedef *me){
		me->mode = 3;
		me->status= OK;
		char* pointer;
		pointer = strtok(NULL, ":");
		pointer = strtok(NULL, ",");
			
		int ref = 0;
		if ((*pointer - '0') == 1) me->buffer[ref++] = 1;
		if ((*pointer - '0') == 0) me->buffer[ref++] = 0;
		if ((*pointer - '0') == 2) me->buffer[ref++] = 2;
		if ((*pointer - '0') == 3) me->buffer[ref++] = 3;
		pointer = strtok(NULL, ",");
		me->buffer[ref] = (*pointer - '0');
		return OK;
}

System_Typedef* System_Ctor(void){
	System_Typedef* me;
	me = (System_Typedef*)malloc(sizeof(System_Typedef));
	me->COMprocess = Package_Ctor();
	me->COMprocess->vptr->_LEDset = _LEDset;
	me->COMprocess->func_pointer[LED_SET] = me->COMprocess->vptr->_LEDset;
	memset(me->SystemLED, 0, 2);
	memset(me->ID, '0', 6);
	return me;
}





void ID_update(System_Typedef* me){
	strcpy(me->ID, me->COMprocess->buffer);
}
void ID_feedback(System_Typedef* me)
{
	strcpy(me->COMprocess->buffer,me->ID);
}
void LED_update(System_Typedef* me)
{
	myLED->cur_pin = (Led_Color_Typedef)me->COMprocess->buffer[0];
	Led_setState(myLED, (Led_State_Typedef)me->COMprocess->buffer[1]);
}
void LED_feedback(System_Typedef* me){
	const char state[2] = {Led_getState(myLED, (Led_Color_Typedef)me->COMprocess->buffer[0]) + '0', '\0'};
	const char Led_pins[2] = {myLED->cur_pin + '0', '\0'};
	memset(me->COMprocess->buffer, 0,50);
	strcpy(me->COMprocess->buffer, "+LED:");
	strcat(me->COMprocess->buffer, Led_pins);
	strcat(me->COMprocess->buffer, ",");
	strcat(me->COMprocess->buffer, state);
}
void doing_nothing(System_Typedef* me){
	__nop();
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	//HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)((struct Package_Typedef*)my_System)->receive_package, 50);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)(my_System->COMprocess->receive_package), 50);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	Package_unfold(my_System->COMprocess);
	memset(my_System->COMprocess->receive_package, 0, 50);
	//memset(my_System->COMprocess->buffer, 0, 50);
	memset(my_System->COMprocess->response_package, 0, 100);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	HAL_UART_DeInit(&huart1);
	HAL_UART_Init(&huart1);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)(my_System->COMprocess->receive_package), 50);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}
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
