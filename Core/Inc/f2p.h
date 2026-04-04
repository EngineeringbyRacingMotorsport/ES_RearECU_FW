/*
 * f2p.h
 *
 *  Created on: Apr 2, 2026
 *      Author: oriol
 */

#ifndef INC_F2P_H_
#define INC_F2P_H_

#include "main.h"

void F2P_Process_Encoders(DICCP_t *DICCP, uint16_t current_L, uint16_t current_R);
void DMA2DICCF(DICCF_t *DICCF);
void DIG2DICCP(DICCP_t *DICCP);
void DMA2DICCP(DICCP_t *DICCP);

#endif /* INC_F2P_H_ */
