/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "common.h"
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
boolean_t getUserButtonState ( void );
void setUserLED1State ( boolean_t state );
void setUserLED2State ( boolean_t state );
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DEBUG_EN_IN_Pin GPIO_PIN_3
#define DEBUG_EN_IN_GPIO_Port GPIOD
#define DEBUG_GPIO_0_Pin GPIO_PIN_2
#define DEBUG_GPIO_0_GPIO_Port GPIOI
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_13
#define LD2_GPIO_Port GPIOI
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define MEMS_AIN_2_Pin GPIO_PIN_8
#define MEMS_AIN_2_GPIO_Port GPIOF
#define MEMS_AIN_1_Pin GPIO_PIN_0
#define MEMS_AIN_1_GPIO_Port GPIOC
#define VCP_TX_Pin GPIO_PIN_10
#define VCP_TX_GPIO_Port GPIOB
#define VCP_RX_Pin GPIO_PIN_11
#define VCP_RX_GPIO_Port GPIOB
#define MEMS_AIN_5_Pin GPIO_PIN_2
#define MEMS_AIN_5_GPIO_Port GPIOC
#define MEMS_AIN_6_Pin GPIO_PIN_3
#define MEMS_AIN_6_GPIO_Port GPIOC
#define MEMS_AIN_3_Pin GPIO_PIN_0
#define MEMS_AIN_3_GPIO_Port GPIOA
#define MEMS_AIN_4_Pin GPIO_PIN_1
#define MEMS_AIN_4_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_2
#define LD1_GPIO_Port GPIOJ
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
