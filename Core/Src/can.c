/*
 * can.c
 *
 *  Created on: Apr 2, 2026
 *      Author: oriol
 */
#include <can.h>

// Variables privades del buffer circular
static CAN_Msg rxBuffer[CAN_RX_BUF_SIZE];
static volatile int rxHead = 0;
static volatile int rxTail = 0;

// Inicialització completa: Filtres + Notificacions + Start
void CAN_Init_Custom(FDCAN_HandleTypeDef *hfdcan) {
    FDCAN_FilterTypeDef sFilterConfig;

    // 1. Configuració de filtre per acceptar-ho TOT
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x7FF;

    if (HAL_FDCAN_ConfigFilter(hfdcan, &sFilterConfig) != HAL_OK) Error_Handler();

    // 2. Activar la interrupció de la FIFO 0
    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) Error_Handler();

    // 3. Arrencar el perifèric
    if (HAL_FDCAN_Start(hfdcan) != HAL_OK) Error_Handler();
}

// Aquesta és la funció que crida el processador automàticament
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0) {
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxBuffer[rxHead].header, rxBuffer[rxHead].data) == HAL_OK) {
            int nextHead = (rxHead + 1) % CAN_RX_BUF_SIZE;
            if (nextHead != rxTail) { // Si no està ple
                rxHead = nextHead;
            }
        }
    }
}

// Les teves funcions d'enviament i creació (CAN_Send, CAN_Msg_Maker...)
HAL_StatusTypeDef CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint32_t len) {
    FDCAN_TxHeaderTypeDef txHeader;
    txHeader.Identifier = id;
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.DataLength = len;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;

    return HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, data);
}

void CAN_Msg_Maker(SIGNAL_dicc *DICCP, uint8_t *Msg1, uint8_t *Msg2)
{
	for (int i = 0; i < 7; i++) {
        Msg1[i] = 0;
        Msg2[i] = 0;
    }

	/* ================ MISSATGE 1 ================ */
	Msg1[0] |= (DICCP->FpANLRpot   & 0xFF);
	Msg1[1] |= (DICCP->FpANLLpot   & 0xFF);
	Msg1[2] |= (DICCP->FpANLRsus   & 0xFF);
	Msg1[3] |= (DICCP->FpANLLsus   & 0xFF);
	Msg1[4] |= (DICCP->FpANLRspeed & 0xFF);
	Msg1[5] |= (DICCP->FpANLLspeed & 0xFF);
	Msg1[6] |= (DICCP->FpANLbrake  & 0xFF);

	/* ================ MISSATGE 2 ================ */
	Msg2[0] |= ((DICCP->FpINTbmseled & 0x01) << 0);
	Msg2[0] |= ((DICCP->FpINTimdeled & 0x01) << 1);
	Msg2[0] |= ((DICCP->FpINTtsoff   & 0x01) << 2);
	Msg2[0] |= ((DICCP->FpINTbmssled & 0x01) << 3);
	Msg2[0] |= ((DICCP->FpINTbutpre  & 0x01) << 4);
	Msg2[0] |= ((DICCP->FpINTbutr2d  & 0x01) << 5);
	Msg2[0] |= ((DICCP->FpINTbutmenu & 0x01) << 6);
	Msg2[0] |= ((DICCP->FpDITmicrosd & 0x01) << 7);

	Msg2[1] |= ((DICCP->FpSDCinertia & 0x01) << 0);
	Msg2[1] |= ((DICCP->FpSDCbots    & 0x01) << 1);
	Msg2[1] |= ((DICCP->FpSDCcsdb    & 0x01) << 2);
	Msg2[1] |= ((DICCP->FpERRapps    & 0x01) << 3);
	Msg2[1] |= ((DICCP->FpDIGrefri   & 0x03) << 4);
	Msg2[1] |= ((DICCP->FpDIGr2d     & 0x01) << 6);

	Msg2[2] |= ((DICCP->FpANLspeed   & 0xFF) << 0);
	Msg2[3] |= ((DICCP->FpANLtempaccu & 0xFF) << 0);
	Msg2[4] |= ((DICCP->FpANLVaccu    & 0xFF) << 0);

	/* 16 bits little endian */
	Msg2[5] |= ((DICCP->FpSHU & 0x00FF) << 0);
	Msg2[6] |= ((DICCP->FpSHU & 0xFF00) >> 8);
}

uint8_t CAN_Read(SIGNAL_dicc *DICCP)
{
    if (rxHead == rxTail) {
        return 0; // No hi ha missatges
    }

    CAN_Msg *rxMsg = &rxBuffer[rxTail];
    uint8_t *data = rxMsg->data;

    switch (rxMsg->header.Identifier)
    {
        case 0x120: // ID del Missatge 2

            // Byte 0: Booleans i estats
            DICCP->SpERRbms    = (data[0] >> 0) & 0x01;
            DICCP->SpERRimd    = (data[0] >> 1) & 0x01;
            DICCP->SpDIGresbut = (data[0] >> 2) & 0x01;
            DICCP->SpSDC       = (data[0] >> 3) & 0x01;

            // SHU (segons el teu codi original)
            DICCP->SpSHU = ((uint16_t)data[2] | ((uint16_t)data[1] << 8)) / 10;

            break;

        default:
            // ID desconeguda → no fem res
            break;
    }

    // Avançar el tail després de processar
    rxTail = (rxTail + 1) % CAN_RX_BUF_SIZE;

    return 1; // Missatge processat
}

