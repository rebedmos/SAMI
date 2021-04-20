/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "fatfs.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <esp8266.h>
#include <esp8266_ll_template.h>
#include <stdio.h>
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
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart1;
char writeValue[60];
uint8_t key, cnt;
int16_t operator1, operator2, result;
char operatorChar[5];
uint8_t transmitir;
uint32_t bytes_read;
uint8_t transmitido_anterior;
uint8_t lernovo;
uint8_t read_image[2048];
uint8_t opChoice = 0;
ESP8266_t ESP8266;
ESP8266_APConfig_t ESP8266_config;
ESP8266_Connection_t* ConnectionGlobal;
volatile StateMachine_TypeDef main_state;
uint8_t rx_index = 0;
uint8_t rx_data;
uint8_t rx_buffer[392];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rx_index == 0)
	{
		for(int i=0; i<392; i++)
		{
			rx_buffer[i] = 0;
		}
	}
	rx_buffer[rx_index++] = rx_data;
	ESP8266_DataReceived(&rx_data, 1);
	HAL_UART_Receive_IT(&huart1, &rx_data, 1);
	__NOP();
}

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
	MX_FATFS_Init();
	MX_USB_HOST_Init();
	MX_USART1_UART_Init();
	MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

	HAL_UART_Receive_IT(&huart1, &rx_data, 1);
	HAL_TIM_Base_Start_IT(&htim6);


	while (ESP8266_Init(&ESP8266, 115200) != ESP_OK);		//Inicializar módulo ESP8266
	HAL_Delay(2000);

	ESP8266_WifiConnect(&ESP8266, "SAMI", "SAMI");	// Conectar no roteador wifi
	HAL_Delay(2000);
	ESP8266_WaitReady(&ESP8266);


	main_state = INIT_ESP_STATE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  MX_USB_HOST_Process();
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
	  cam_image();
	  ESP8266_Update(&ESP8266);	//atualizar a stack tcp/ip constantemente

	  switch (main_state)
	  {
			case INIT_ESP_STATE:
			{
				/* Wait till finish */
				ESP8266_WaitReady(&ESP8266);
				while (ESP8266_StartClientConnectionTCP(&ESP8266,"SAMI","192.168.1.121",8080,NULL)!= ESP_OK);
				/* Wait till finish */
				ESP8266_WaitReady(&ESP8266);

				main_state = SEND_DATA_STATE;
			}
			break;

			case SEND_DATA_STATE:
			{
				//indica que o anterior nao foi enviado
				if((rx_buffer[rx_index-7] =='E')&&(rx_buffer[rx_index-6] =='R')&&(rx_buffer[rx_index-5] =='R')&&(rx_buffer[rx_index-4] =='O')&&(rx_buffer[rx_index-3] =='R'))
				{
					transmitido_anterior = 0;
					HAL_Delay(20);
				}
				if(transmitir)	//pode transmitir?
				{
					ESP8266_WaitReady(&ESP8266);
					while (ESP8266_RequestSendData(&ESP8266,ConnectionGlobal)!= ESP_OK);	//requisitar autorizacao de envio de dados
					ESP8266_WaitReady(&ESP8266);

					//request send data retornou fail?
					if((rx_buffer[rx_index-6] =='F') && (rx_buffer[rx_index-5] == 'A') && (rx_buffer[rx_index-4] == 'I') && (rx_buffer[rx_index-3] == 'L') && (rx_buffer[rx_index-20] == '0'))	//se requisição falhou e não recebeu nenhum byte
					{
						transmitido_anterior = 0;	//nao transmitiu os dados atuais
						lernovo = 0;				//nao atualizar o buffer antes de ser enviado
					}
					else
					{
						ESP8266_LL_USARTSend(&read_image, bytes_read);	//enviar dados
						ESP8266_WaitReady(&ESP8266);
						ESP8266_LL_USARTSend("\r\n", 2);				//concluir envio de dados
						ESP8266_WaitReady(&ESP8266);
						transmitido_anterior = 1;						//transmitido com sucesso
						lernovo = 1;									//solicitar atualizacao de buffer
					}
				}
				HAL_Delay(20);
				main_state = SEND_DATA_STATE;
			}
			break;

			case DEINIT_ESP_STATE:
			{
				ESP8266_WaitReady(&ESP8266);
				while (ESP8266_CloseAllConnections(&ESP8266)!= ESP_OK);
				ESP8266_WaitReady(&ESP8266);
				main_state = INIT_ESP_STATE;
			}
				break;
			default:
				break;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USB;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 26667;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 2;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WIFI_RSTn_GPIO_Port, WIFI_RSTn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : WIFI_RSTn_Pin */
  GPIO_InitStruct.Pin = WIFI_RSTn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(WIFI_RSTn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	__NOP();
}

//a cada 1 ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    ESP8266_TimeUpdate(&ESP8266, 1);
}


/************************************/
/*           ESP CALLBACKS          */
/************************************/
/* Called when ready string detected */
void ESP8266_Callback_DeviceReady(ESP8266_t* ESP8266) {
//	printf("Device is ready\r\n");
}

/* Called when watchdog reset on ESP8266 is detected */
void ESP8266_Callback_WatchdogReset(ESP8266_t* ESP8266) {
//	printf("Watchdog reset detected!\r\n");
}

/* Called when we are disconnected from WIFI */
void ESP8266_Callback_WifiDisconnected(ESP8266_t* ESP8266) {
//	printf("Wifi is disconnected!\r\n");
}

void ESP8266_Callback_WifiConnected(ESP8266_t* ESP8266) {
//	printf("Wifi is connected!\r\n");
}

void ESP8266_Callback_WifiConnectFailed(ESP8266_t* ESP8266) {
//	printf("Connection to wifi network has failed. Reason %d\r\n", ESP8266->WifiConnectError);
}

void ESP8266_Callback_WifiGotIP(ESP8266_t* ESP8266) {
//	printf("Grabbing IP status: %d\r\n", ESP8266_GetSTAIP(ESP8266));
}

void ESP8266_Callback_WifiIPSet(ESP8266_t* ESP8266) {
	/* We have STA IP set (IP set by router we are connected to) */
//	printf("We have valid IP address: %d.%d.%d.%d\r\n", ESP8266->STAIP[0], ESP8266->STAIP[1], ESP8266->STAIP[2], ESP8266->STAIP[3]);
}

void ESP8266_Callback_DHCPTimeout(ESP8266_t* ESP8266) {
//	printf("DHCP timeout!\r\n");
}

void ESP8266_Callback_WifiDetected(ESP8266_t* ESP8266, ESP8266_APs_t* ESP8266_AP) {
//	uint8_t i = 0;
//
//	/* Print number of detected stations */
//	printf("We have detected %d AP stations\r\n", ESP8266_AP->Count);
//
//	/* Print each AP */
//	for (i = 0; i < ESP8266_AP->Count; i++) {
//		/* Print SSID for each AP */
//		printf("%2d: %s\r\n", i, ESP8266_AP->AP[i].SSID);
//	}
}


/************************************/
/*         CLIENT CALLBACKS         */
/************************************/
void ESP8266_Callback_ClientConnectionConnected(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection) {
//	/* We are connected to external server */
//	printf(writeValue, "Client connected to server! Connection name: %s\r\n", Connection->Name);
//
//	ConnectionGlobal = Connection;
//	/* We are connected to server, request to sent header data to server */
//	ESP8266_RequestSendData(ESP8266, Connection);
}

/* Called when client connection fails to server */
void ESP8266_Callback_ClientConnectionError(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection) {
	/* Fail with connection to server */
//	printf("An error occurred when trying to connect on connection: %d\r\n", Connection->Number);
}

/* Called when data are ready to be sent to server */
uint16_t ESP8266_Callback_ClientConnectionSendData(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection, char* Buffer, uint16_t max_buffer_size) {

//	/* Format data to sent to server */
//
//
//
//	sprintf(Buffer, "Msg Teste: %d", msgCnt);
//	strcat(Buffer, "\r\n");
//
//	msgCnt++;
//	/* Return length of buffer */
//	return strlen(Buffer);

}

/* Called when data are send successfully */
void ESP8266_Callback_ClientConnectionDataSent(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection) {
//	printf("Data successfully sent as client!\r\n");
}

/* Called when error returned trying to sent data */
void ESP8266_Callback_ClientConnectionDataSentError(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection) {

//	printf("Error while sending data on connection %d!\r\n", Connection->Number);

}

void ESP8266_Callback_ClientConnectionDataReceived(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection, char* Buffer) {
//	/* Data received from server back to client */
//	printf("Data received from server on connection: %s; Number of bytes received: %u; %u / %u;\r\n", Connection->Name, Connection->BytesReceived, Connection->TotalBytesReceived, Connection->ContentLength);
//	printf("Message received: \r\n");
//	printf("%s \r\n",Connection->Data);
}

/* Called when connection is closed */
void ESP8266_Callback_ClientConnectionClosed(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection) {
//	printf("Client connection closed, connection: %d; Total bytes received: %u; Content-Length header: %u\r\n", Connection->Number, Connection->TotalBytesReceived, Connection->ContentLength);
}

/* Called when timeout is reached on connection to server */
void ESP8266_Callback_ClientConnectionTimeout(ESP8266_t* ESP8266, ESP8266_Connection_t* Connection) {
//	printf("Timeout reached on connection: %d\r\n", Connection->Number);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
