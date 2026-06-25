/*
 * f2p.c
 *
 *  Created on: Jun 25, 2026
 *      Author: oriol
 */

#include "f2p.h"

void DMA2DICCF(DICCF_t *DICCF, uint32_t *buffer) {
	DICCF->RfSHU = buffer[0];
	DICCF->RfSIGlvs = buffer[1];
}

void DIG2DICCF(DICCF_t *DICCF){
	DICCF->RfSDChvd = HAL_GPIO_ReadPin(GPIOB, RfSDChvd_Pin);
	DICCF->RfSDCtsms = HAL_GPIO_ReadPin(GPIOB, RfSDCtsms_Pin);
	DICCF->RfSDClsdb = HAL_GPIO_ReadPin(GPIOB, RfSDClsdb_Pin);
	DICCF->RfSDCrsdb = HAL_GPIO_ReadPin(GPIOB, RfSDCrsdb_Pin);
	DICCF->RfSTAbrkledR = HAL_GPIO_ReadPin(GPIOA, RfSTAbrkledR_Pin);
	DICCF->RfSTAbrkledG = HAL_GPIO_ReadPin(GPIOA, RfSTAbrkledG_Pin);
	DICCF->RfSTAbrkledB = HAL_GPIO_ReadPin(GPIOA, RfSTAbrkledB_Pin);
	DICCF->RfSTArefriaccu = HAL_GPIO_ReadPin(GPIOC, RfSTArefriaccu_Pin);
	DICCF->RfSTArefrimot = HAL_GPIO_ReadPin(GPIOC, RfSTArefrimot_Pin);
	DICCF->RfSTArefriinverter = HAL_GPIO_ReadPin(GPIOC, RfSTArefriinverter_Pin);
}

void DICCF2DICCP(DICCF_t *DICCF, DICCP_t *DICCP) {
	DICCP->RpSDChvd = !DICCF->RfSDChvd;
	DICCP->RpSDCtsms = !DICCF->RfSDCtsms;
	DICCP->RpSDClsdb = !DICCF->RfSDClsdb;
	DICCP->RpSDCrsdb = !DICCF->RfSDCrsdb;

	DICCP->RpSTAbrkledR = DICCF->RfSTAbrkledR;
	DICCP->RpSTAbrkledG = DICCF->RfSTAbrkledG;
	DICCP->RpSTAbrkledB = DICCF->RfSTAbrkledB;
	DICCP->RpSTArefriaccu = DICCF->RfSTArefriaccu;
	DICCP->RpSTArefrimot = DICCF->RfSTArefrimot;
	DICCP->RpSTArefriinverter = DICCF->RfSTArefriinverter;

	DICCP->RpSIGlvs = ((DICCF->RfSIGlvs*3.3)/2095)*9.195;
	DICCP->RpSHU = ((DICCF->RfSHU*3.3)/2095)/(0.13*100)*1000;

	DICCP->IpRPM = (DICCF->IfRPM * 5600) / 20000;
	DICCP->IpI = (DICCF->IfI * 235) / 2000;
	DICCP->IpV = DICCF->IfV * 10;
	DICCP->IpPar = (DICCF->IfPar / 20) + 1000;
	DICCP->IpT_IGBT = DICCF->IfT_IGBT;
	DICCP->IpT_Mot = DICCF->IfT_Mot;
	DICCP->IpErrL = DICCF->IfErr & 0xFFFF;
	DICCP->IpErrH = (DICCF->IfErr >> 16) & 0xFFFF;
}
