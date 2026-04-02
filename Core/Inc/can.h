/*
 * can.h
 *
 *  Created on: Apr 2, 2026
 *      Author: oriol
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

#define CAN_RX_BUF_SIZE 64

typedef struct {
    FDCAN_RxHeaderTypeDef header;
    uint8_t data[8];
} CAN_Msg;

typedef struct {
	/* ================ MISSATGE 1 ================ */
	uint8_t FpANLRpot;
	uint8_t FpANLLpot;
	uint16_t FpANLRsus;
	uint16_t FpANLLsus;
	uint8_t FpANLRspeed;
	uint8_t FpANLLspeed;
	uint8_t FpANLbrake;

	/* ================ MISSATGE 1 ================ */
	uint8_t FpINTbmseled;
	uint8_t FpINTimdeled;
	uint8_t FpINTtsoff;
	uint8_t FpINTbmssled;
	uint8_t FpINTbutpre;
	uint8_t FpINTbutr2d;
	uint8_t FpINTbutmenu;
	uint8_t FpDITmicrosd;
	uint8_t FpSDCinertia;
	uint8_t FpSDCbots;
	uint8_t FpSDCcsdb;
	uint8_t FpERRapps;
	uint8_t FpDIGrefri;
	uint8_t FpDIGr2d;
	uint8_t FpANLspeed;
	uint8_t FpANLtempaccu;
	uint8_t FpANLVaccu;
	uint16_t FpSHU;

	/* ================ MISSATGE 1 ================ */
	uint8_t SpERRbms;
	uint8_t SpERRimd;
	uint8_t SpDIGresbut;
	uint8_t SpSDC;
	uint16_t SpSHU;

	/* ================ MISSATGE 1 ================ */

} SIGNAL_dicc;

/* Prototips actualitzats per a FDCAN i amb la signatura correcta */
void CAN_Init_Custom(FDCAN_HandleTypeDef *hfdcan);
HAL_StatusTypeDef CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint32_t len);
uint8_t CAN_Read(SIGNAL_dicc *DICCP);
void CAN_Msg_Maker(SIGNAL_dicc *DICCP, uint8_t *Msg1, uint8_t *Msg2);

#endif /* INC_CAN_H_ */
