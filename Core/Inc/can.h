/*
 * can.h
 *
 *  Created on: Jun 25, 2026
 *      Author: oriol
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "main.h"

void CAN_Msg_Maker(DICCP_t *DICCP, uint8_t *Msg);
void CAN_Init_Custom(FDCAN_HandleTypeDef *hfdcan);
HAL_StatusTypeDef CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint32_t len);

#endif /* INC_CAN_H_ */
