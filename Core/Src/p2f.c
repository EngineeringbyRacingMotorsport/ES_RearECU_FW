#include "p2f.h"
#include "main.h"

uint8_t DifTempInv = 20;
uint8_t DifTempMot = 20;

void INVERTER(DICCP_t *DICCP){
	uint8_t switch_state = 0;
	uint8_t SDC = DICCP->DpSDC;
	uint8_t R2D = DICCP->FpDIGr2d;
	switch (switch_state)
		{
			// Condicions per passar d'inicial a STEP1: fre premut (Brake >= 300), SDC actiu (SDC != 0), botó R2D premut (Button_R2DX != 0),
			//Aire positiu OK (TSoff != 0) i sense error d'APPS (!error_apps)
			case 0:
				if (SDC == 1){
					switch_state = 1;
				}
				break;

			// En STEP1 exigim: Fre continua premut, SDC actiu, botó alliberat (!Button_R2DX), air positiu OK i sense error APPS
			case 1:
				if (R2D == 1 && SDC == 1){			// HAL_GetTick() dona el nombre de mil·lisegons des de HAL_Init() (és un contador global de temps del sistema incrementat per l'interrupt de SysTick).
					switch_state = 2;					// Passem a STEP2 (finestra d'espera de 2 segons)
				}
				else if (SDC == 0){
					switch_state = 0;
				}
				break;

			// En STEP2: SDC actiu, botó segueix alliberat, air positiu OK, sense error APPS i han passat com a mínim 2000 ms (2 s) des de temp_R2D
			case 2:
				if (R2D == 1 && SDC == 1){			// HAL_GetTick() dona el nombre de mil·lisegons des de HAL_Init() (és un contador global de temps del sistema incrementat per l'interrupt de SysTick).
					switch_state = 3;					// Passem a STEP2 (finestra d'espera de 2 segons)
				}
				else if (R2D == 0 || SDC == 0){		// Aquí, determinem el temps que s'ha avançat respecte el punt inicial
					switch_state = 0;																			// Si es compleix tot això, passem a STEP3
				}
				break;
			case 3:
				if (R2D == 0 || SDC == 0){		// Aquí, determinem el temps que s'ha avançat respecte el punt inicial
					switch_state = 0;																			// Si es compleix tot això, passem a STEP3
				}
				break;
		}

	if (switch_state == 0 || switch_state == 1)
		{
			DICCP->REGID   = 0x51;
			DICCP->INVdata = 4;
		}
	else if (switch_state == 2)
		{
			DICCP->REGID   = 0x51;
			DICCP->INVdata = 0;
		}
	else if (switch_state == 3)
		{
			DICCP->REGID   = 0x90;
			DICCP->INVdata = ((DICCP->FpANLRpot)+(DICCP->FpANLLpot)/2)*327.67;
		}
	else
	{
		DICCP->REGID   = 0x51;
		DICCP->INVdata = 4;
	}
}

void PLC(DICCP_t *DICCP)
{
	if(DifTempInv <= (DICCP->RpSIGOtempI - DICCP->RpSIGItempI))
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriinverter_Pin, GPIO_PIN_SET); }
	else
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriinverter_Pin, GPIO_PIN_RESET); }

	if(DifTempMot <= (DICCP->RpSIGOtempM - DICCP->RpSIGItempM))
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefrimot_Pin, GPIO_PIN_SET); }
	else
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefrimot_Pin, GPIO_PIN_RESET); }
	/*
	if()
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriaccu_Pin, GPIO_PIN_SET); }
	else
	{ HAL_GPIO_WritePin(GPIOC, RfSTArefriaccu_Pin, GPIO_PIN_RESET); }

	if()
	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledR_Pin, GPIO_PIN_SET); }
	else
	{ HAL_GPIO_WritePin(GPIOA, RfSTAbrkledR_Pin, GPIO_PIN_RESET); }
*/
}

