/*
 * f2p.c
 *
 *  Created on: Apr 2, 2026
 *      Author: oriol
 */
#include "f2p.h"

#define ADC1_CH 6 // Definim 8 perquè son els espais a la array contant l'espai 0 com un, es a dir que la array serà de [0-7].
#define ADC2_CH 2

extern uint16_t DICCDMA[ADC1_CH + ADC2_CH];

static uint16_t prev_L = 0;
static uint16_t prev_R = 0;

void F2P_Process_Encoders(DICCP_t *DICCP, uint16_t current_L, uint16_t current_R) {

	int16_t delta_L = (int16_t)(current_L - prev_L);
	int16_t delta_R = (int16_t)(current_R - prev_R);

	prev_L = current_L;
	prev_R = current_R;

	float factor = (3.14159 * /* Diametre del encoder ex: 20cm */0.2 * 3.6) / (/*Numero de forats del encoder*/1024 * 0.1/*Temps de cada salt del TIM6 en segons*/);

	DICCP->RpSIGLspeed = (uint16_t)(delta_L * factor);
	DICCP->RpSIGRspeed = (uint16_t)(delta_R * factor);
}

void DMA2DICCF(DICCF_t *DICCF) {
	for(int i = 0; i < ADC1_CH; i++) {
		DICCF->array[i] = (float)DICCDMA[i];  //D moment així prq encara no sé com ha de ser la conversió
	}

	for(int i = ADC1_CH; i < (ADC1_CH + ADC2_CH); i++) {
		DICCF->array[i] = (float)DICCDMA[i];  //D moment així prq encara no sé com ha de ser la conversió
	}
}

void DIG2DICCP(DICCP_t *DICCP){
	DICCP->RpSDChvd = HAL_GPIO_ReadPin(GPIOB, RfSDChvd_Pin);
	DICCP->RpSDCtsms = HAL_GPIO_ReadPin(GPIOB, RfSDCtsms_Pin);
	DICCP->RpSDClsdb = HAL_GPIO_ReadPin(GPIOB, RfSDClsdb_Pin);
	DICCP->RpSDCrsdb = HAL_GPIO_ReadPin(GPIOC, RfSDCrsdb_Pin);
}

void DMA2DICCP(DICCP_t *DICCP) {
	DICCP->RpSIGItempI = DICCF.RfSIGItempI; //Conversió
	DICCP->RpSIGOtempI = DICCF.RfSIGOtempI; //Conversió
	DICCP->RpSIGItempM = DICCF.RfSIGItempM; //Conversió
	DICCP->RpSIGOtempM = DICCF.RfSIGOtempM; //Conversió
	DICCP->RpSIGLsus = DICCF.RfSIGLsus; //Conversió
	DICCP->RpSIGRsus = DICCF.RfSIGRsus; //Conversió
	DICCP->RSIGlvs = DICCF.RSIGlvs; //Conversió
	DICCP->RpSHU = DICCF.RfSHU; //Conversió
}

