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

DICCF_t DICCF;
DICCP_t DICCP;

void DMA2DICCF(void) {
	for(int i = 0; i < ADC1_CH; i++) {
		DICCF.array[i] = (float)DICCDMA[i];  //D moment així prq encara no sé com ha de ser la conversió
	}

	for(int i = ADC1_CH; i < (ADC1_CH + ADC2_CH); i++) {
		DICCF.array[i] = (float)DICCDMA[i];  //D moment així prq encara no sé com ha de ser la conversió
	}
}
void DIG2DICCF(void){
	DICCP.RpSDChvd = HAL_GPIO_ReadPin(GPIOB, RfSDChvd_Pin);
	DICCP.RpSDCtsms = HAL_GPIO_ReadPin(GPIOB, RfSDCtsms_Pin);
	DICCP.RpSDClsdb = HAL_GPIO_ReadPin(GPIOB, RfSDClsdb_Pin);
	DICCP.RpSDCrsdb = HAL_GPIO_ReadPin(GPIOC, RfSDCrsdb_Pin);
}

void DMA2DICCP(void) {
	DICCP.RpSIGItempI = DICCF.RfSIGItempI; //Conversió
	DICCP.RpSIGOtempI = DICCF.RfSIGOtempI; //Conversió
	DICCP.RpSIGItempM = DICCF.RfSIGItempM; //Conversió
	DICCP.RpSIGOtempM = DICCF.RfSIGOtempM; //Conversió
	DICCP.RpSIGLsus = DICCF.RfSIGLsus; //Conversió
	DICCP.RpSIGRsus = DICCF.RfSIGRsus; //Conversió
	DICCP.RSIGlvs = DICCF.RSIGlvs; //Conversió
	DICCP.RpSHU = DICCF.RfSHU; //Conversió
}

