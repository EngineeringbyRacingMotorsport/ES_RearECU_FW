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

void CAN_Msg_Maker(DICCP_t *DICCP, uint8_t *Msg1, uint8_t *Msg2, uint8_t *MsgInv){
	/* ================ MISSATGE 1 ================ */
	Msg1[0] |= (DICCP->RpSIGRsus   & 0xFF);
	Msg1[1] |= (DICCP->RpSIGLsus   & 0xFF);
	Msg1[2] |= (DICCP->RpSIGRspeed & 0xFF);
	Msg1[3] |= (DICCP->RpSIGLspeed & 0xFF);
	Msg1[4] |= (DICCP->RpSIGOtempM & 0xFF);
	Msg1[5] |= (DICCP->RpSIGItempM & 0xFF);
	Msg1[6] |= (DICCP->RpSIGItempI & 0xFF);
	Msg1[7] |= (DICCP->RpSIGOtempI & 0xFF);

	/* ================ MISSATGE 2 ================ */
	Msg2[0] |= ((DICCP->RpSDChvd  & 0x01) << 0);
	Msg2[0] |= ((DICCP->RpSDCtsms & 0x01) << 1);
	Msg2[0] |= ((DICCP->RpSDCrsdb & 0x01) << 2);
	Msg2[0] |= ((DICCP->RpSDClsdb & 0x01) << 3);

	Msg2[1] |= ((DICCP->RpSTAbrkledR   & 0x01) << 0);
	Msg2[1] |= ((DICCP->RpSTAbrkledG   & 0x01) << 1);
	Msg2[1] |= ((DICCP->RpSTAbrkledB   & 0x01) << 2);
	Msg2[1] |= ((DICCP->RpSTArefriaccu & 0x01) << 3);
	Msg2[1] |= ((DICCP->RpSTArefrimot  & 0x01) << 4);
	Msg2[1] |= ((DICCP->RpSTArefriinv  & 0x01) << 6);

	Msg2[2] |= ((DICCP->RSIGlvs & 0x00FF) << 0);
	Msg2[3] |= ((DICCP->RSIGlvs & 0xFF00) >> 8);

	Msg2[4] |= ((DICCP->RpSHU & 0x00FF) << 0);
	Msg2[5] |= ((DICCP->RpSHU & 0xFF00) >> 8);

	/* ================ MISSATGE INVERTER ================ */
	MsgInv[0] |= (DICCP->REGID & 0xFF);

	MsgInv[1] |= ((DICCP->INVdata & 0x00FF) << 0);
	MsgInv[2] |= ((DICCP->INVdata & 0xFF00) >> 8);

}

uint8_t CAN_Read(DICCP_t *DICCP){
    if (rxHead == rxTail) {
        return 0; // No hi ha missatges
    }

    CAN_Msg *rxMsg = &rxBuffer[rxTail];
    uint8_t *data = rxMsg->data;

    switch (rxMsg->header.Identifier)
    {
        case 0x120: // MISSATGE DE LA FRONT ECU

            // Byte 0: Booleans i estats
            DICCP->FpANLRpot   = data[0] & 0xFF;
            DICCP->FpANLRpot   = data[1] & 0xFF;

            DICCP->FpDIGr2d    = (data[1] >> 6) & 0x01;

            break;

        default:
            // ID desconeguda → no fem res
            break;
    }

    // Avançar el tail després de processar
    rxTail = (rxTail + 1) % CAN_RX_BUF_SIZE;

    return 1; // Missatge processat
}
