/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
typedef enum
{
	INIT_ESP_STATE		= 0,
	SEND_DATA_STATE		= 1,
	DEINIT_ESP_STATE	= 2,
	WRONG_STATE	= 255
}StateMachine_TypeDef;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WIFI_RSTn_Pin GPIO_PIN_1
#define WIFI_RSTn_GPIO_Port GPIOC
#define WIFI_uC_TX_Pin GPIO_PIN_9
#define WIFI_uC_TX_GPIO_Port GPIOA
#define WIFI_uC_RX_Pin GPIO_PIN_7
#define WIFI_uC_RX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
extern UART_HandleTypeDef huart1;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
