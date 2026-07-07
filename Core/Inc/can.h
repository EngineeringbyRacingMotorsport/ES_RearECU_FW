/*
 * can.h
 *
 *  Created on: Jun 25, 2026
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

uint8_t CAN_Read(volatile DICCP_t *DICCP);

void CAN_Msg_Maker(volatile DICCP_t *DICCP, uint8_t *Msg1, uint8_t *Msg2, uint8_t *Msg3);
void CAN_Init_Custom(FDCAN_HandleTypeDef *hfdcan);
HAL_StatusTypeDef CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint32_t len);
void Inverter_Request_Data(FDCAN_HandleTypeDef *hcan, uint8_t regID, uint8_t interval_ms);
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

#endif /* INC_CAN_H_ */
