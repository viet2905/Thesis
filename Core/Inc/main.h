/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Trig_T3_CH3_Pin GPIO_PIN_4
#define Trig_T3_CH3_GPIO_Port GPIOE
#define Trig_T3_CH1_Pin GPIO_PIN_4
#define Trig_T3_CH1_GPIO_Port GPIOC
#define Trig_T3_CH2_Pin GPIO_PIN_5
#define Trig_T3_CH2_GPIO_Port GPIOC
#define Trig_T3_CH4_Pin GPIO_PIN_7
#define Trig_T3_CH4_GPIO_Port GPIOE
#define EnA1_Pin GPIO_PIN_9
#define EnA1_GPIO_Port GPIOE
#define EnB1_Pin GPIO_PIN_11
#define EnB1_GPIO_Port GPIOE
#define PWM_Right_Pin GPIO_PIN_10
#define PWM_Right_GPIO_Port GPIOB
#define PWM_Left_Pin GPIO_PIN_11
#define PWM_Left_GPIO_Port GPIOB
#define EnA2_Pin GPIO_PIN_6
#define EnA2_GPIO_Port GPIOC
#define EnB2_Pin GPIO_PIN_7
#define EnB2_GPIO_Port GPIOC
#define R2_Pin GPIO_PIN_3
#define R2_GPIO_Port GPIOD
#define R1_Pin GPIO_PIN_5
#define R1_GPIO_Port GPIOD
#define STBY_Pin GPIO_PIN_7
#define STBY_GPIO_Port GPIOD
#define Trig_T4_CH2_Pin GPIO_PIN_3
#define Trig_T4_CH2_GPIO_Port GPIOB
#define L1_Pin GPIO_PIN_4
#define L1_GPIO_Port GPIOB
#define L2_Pin GPIO_PIN_6
#define L2_GPIO_Port GPIOB
#define Trig_T4_CH3_Pin GPIO_PIN_9
#define Trig_T4_CH3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
