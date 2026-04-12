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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define ADC1_CH 6 //Pot ser que hi hagin 2 canals de ADC per el DMA ja que cada ADC te les sebes entrades
#define ADC2_CH 2
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//Unió de Array amb Struct, cada espai de la array es un punt de la struct (Ex: array[3] = RfSIGItempM)
typedef union {
    uint16_t array[ADC1_CH + ADC2_CH];
    struct {
    	uint16_t RfSIGOtempI;
    	uint16_t RfSIGItempI;
    	uint16_t RfSIGOtempM;
    	uint16_t RfSIGItempM;
    	uint16_t RfSIGLsus;
    	uint16_t RfSIGRsus;
    	uint16_t RSIGlvs;
    	uint16_t RfSHU;
    };
} DICCF_t;

extern DICCF_t DICCF;

typedef struct {
	//Analog
	uint16_t RpSIGOtempI;
	uint16_t RpSIGItempI;
	uint16_t RpSIGOtempM;
	uint16_t RpSIGItempM;
	uint16_t RpSIGLsus;
	uint16_t RpSIGRsus;
	uint16_t RSIGlvs;
	uint16_t RpSHU;
	//Digital
	uint8_t RpSDChvd;
	uint8_t RpSDCtsms;
	uint8_t RpSDClsdb;
	uint8_t RpSDCrsdb;
	uint8_t RpSTAbrkledR;
	uint8_t RpSTAbrkledG;
	uint8_t RpSTAbrkledB;
	uint8_t RpSTArefriaccu;
	uint8_t RpSTArefrimot;
	uint8_t RpSTArefriinv;
	//Encoder
	uint16_t RpSIGRspeed;
	uint16_t RpSIGLspeed;
	//Inverter
	uint8_t FpDIGr2d;
	uint8_t DpSDC;
	uint8_t FpANLRpot;
	uint8_t FpANLLpot;
	uint8_t REGID;
	uint8_t INVdata;

} DICCP_t;

extern DICCP_t DICCP;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RfSTArefriaccu_Pin GPIO_PIN_13
#define RfSTArefriaccu_GPIO_Port GPIOC
#define RfSTArefriinverter_Pin GPIO_PIN_14
#define RfSTArefriinverter_GPIO_Port GPIOC
#define RfSTArefrimot_Pin GPIO_PIN_15
#define RfSTArefrimot_GPIO_Port GPIOC
#define RfSIGOtempI_Pin GPIO_PIN_0
#define RfSIGOtempI_GPIO_Port GPIOA
#define RfSIGItempI_Pin GPIO_PIN_1
#define RfSIGItempI_GPIO_Port GPIOA
#define RfSIGOtempM_Pin GPIO_PIN_2
#define RfSIGOtempM_GPIO_Port GPIOA
#define RfSIGItempM_Pin GPIO_PIN_3
#define RfSIGItempM_GPIO_Port GPIOA
#define RfSTAbrkledR_Pin GPIO_PIN_4
#define RfSTAbrkledR_GPIO_Port GPIOA
#define RfSTAbrkledG_Pin GPIO_PIN_5
#define RfSTAbrkledG_GPIO_Port GPIOA
#define RfSTAbrkledB_Pin GPIO_PIN_6
#define RfSTAbrkledB_GPIO_Port GPIOA
#define RfSHU_Pin GPIO_PIN_7
#define RfSHU_GPIO_Port GPIOA
#define RfSIGRsus_Pin GPIO_PIN_0
#define RfSIGRsus_GPIO_Port GPIOB
#define RfSIGLsus_Pin GPIO_PIN_1
#define RfSIGLsus_GPIO_Port GPIOB
#define RfSIGlvs_Pin GPIO_PIN_2
#define RfSIGlvs_GPIO_Port GPIOB
#define RfSDChvd_Pin GPIO_PIN_11
#define RfSDChvd_GPIO_Port GPIOB
#define RfSDCtsms_Pin GPIO_PIN_12
#define RfSDCtsms_GPIO_Port GPIOB
#define RfSDClsdb_Pin GPIO_PIN_13
#define RfSDClsdb_GPIO_Port GPIOB
#define RfSDCrsdb_Pin GPIO_PIN_14
#define RfSDCrsdb_GPIO_Port GPIOB
#define RfSIGLspeed_Pin GPIO_PIN_4
#define RfSIGLspeed_GPIO_Port GPIOB
#define RfSIGRspeed_Pin GPIO_PIN_5
#define RfSIGRspeed_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
