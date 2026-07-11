#include "p2f.h"

void PLC(volatile DICCP_t *DICCP)
{
//	if(DifTempInv <= (DICCP->IpT_IGBT))
//	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriinverter_Pin, GPIO_PIN_SET); }
//	else
//	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriinverter_Pin, GPIO_PIN_RESET); }
//
//	if(DifTempMot <= (DICCP->IpT_Mot))
//	{ HAL_GPIO_WritePin(GPIOC, RfSTArefrimot_Pin, GPIO_PIN_SET); }
//	else
//	{ HAL_GPIO_WritePin(GPIOC, RfSTArefrimot_Pin, GPIO_PIN_RESET); }
//
	if(DICCP->FpDIGr2d == 1)
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriaccu_Pin, GPIO_PIN_SET); }
	else
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriaccu_Pin, GPIO_PIN_RESET); }

	if(DICCP->FpANLbrake >= 5)
	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledR_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, RfSTAbrkledG_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, RfSTAbrkledB_Pin, GPIO_PIN_SET);}
	else
	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledR_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, RfSTAbrkledG_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, RfSTAbrkledB_Pin, GPIO_PIN_RESET);}

//	if(DICCP->RpSTAbrkledG != 0)
//	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledG_Pin, GPIO_PIN_SET); }
//	else
//	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledG_Pin, GPIO_PIN_RESET); }
//
//	if(DICCP->RpSTAbrkledB != 0)
//	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledB_Pin, GPIO_PIN_SET); }
//	else
//	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledB_Pin, GPIO_PIN_RESET); }
}


