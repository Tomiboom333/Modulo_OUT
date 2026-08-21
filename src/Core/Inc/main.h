/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"

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

/* USER CODE BEGIN Private defines */
#define SAL_DIG_1 GPIO_PIN_9
#define SAL_DIG_2 GPIO_PIN_8
#define SAL_DIG_3 GPIO_PIN_7
#define SAL_DIG_4 GPIO_PIN_6
#define SAL_DIG_5 GPIO_PIN_5
#define SAL_DIG_6 GPIO_PIN_4
#define SAL_DIG_7 GPIO_PIN_3
#define SAL_DIG_8 GPIO_PIN_15

#define ENT_DIG_1 GPIO_PIN_12//A
#define ENT_DIG_2 GPIO_PIN_9//A
#define ENT_DIG_3 GPIO_PIN_8//A
#define ENT_DIG_4 GPIO_PIN_15
#define ENT_DIG_5 GPIO_PIN_14
#define ENT_DIG_6 GPIO_PIN_13
#define ENT_DIG_7 GPIO_PIN_12
#define ENT_DIG_8 GPIO_PIN_11

extern uint16_t salidasD[8];
extern uint16_t entradasD[8];

typedef struct 
{
    bool modId[8];
    bool modOd[8];
    uint8_t modIa[2];
    uint8_t modOa[2];
}estAct_t;

extern estAct_t estAct;
extern SPI_HandleTypeDef hspi1;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
