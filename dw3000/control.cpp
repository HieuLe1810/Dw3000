#include "control.h"
// // transmit SP0 pkg - Prepoll and Final Data (SP0 iss data without any STS)
// void DW3000_Transmitframe_SP0(CCC_FRAME frame, uint8_t *data, uint8_t lenght, uint8_t offset, uint8_t cs_pin)
// {
//     bool lRetrunVal = FALSE;
//     DW3000_TX_FCTRL txfctrl;
//     DW3000_CHAN_CTRL txcctrl;
//     switch (frame)
//     {
//     case PREPOLL:
//         // First write our transmit buffer
//         DW3000_writeTXData(lenght, data, 0);
//         // then we config for this to be a valid PrePoll frame
//         // write our config for sending ranging frame
//         memset(&txfctrl, 0x00, sizeof(txfctrl));
//         txfctrl.tx_flen = lenght;
//         txfctrl.tx_br = 1;
//         txfctrl.tx_psr = 1;
//         txfctrl.fine_plen = 0;

//         // read back STS pkg format
//         DW3000_getConfig(data_read_sys_cfg, cs_pin);
//         memcpy(&sys_cfg, data_read_sys_cfg, sizeof(data_read_sys_cfg));
//         Serial.print("sys_cfg.CP_SDC: ");
//         Serial.println(sys_cfg.CP_SDC);
//         // set it to SP3
//         if (sys_cfg.CP_SDC != SP3)
//         {
//             sys_cfg.CP_SDC = SP3;
//         }
//         DW3000_setConfig(&sys_cfg, cs_pin);

//         // Then write the transmit frame config
//         DW3000_writeTXFCTRL(&txfctrl);

//         // update CHN_CONFIG

//         memset(&txcctrl, 0x00, sizeof(txcctrl));
//         txcctrl.RF_CHAN = 1;
//         txcctrl.SFD_TYPE = 0x3;
//         txcctrl.TX_PCODE = 0x9;
//         txcctrl.RX_PCODE = 0x9;

//         // Write to Channel control
//         sys_status_controlDW3000_writeCHAN_CTRL(&txcctrl, cs_pin);
//         lRetrunVal = TRUE;
//         break;

//     case FINALDATA:
//         // First write our transmit buffer
//         DW3000_writeTXData(lenght, data, 0);
//         // then we config for this to be a valid PrePoll frame
//         // write our config for sending ranging frame
//         memset(&txfctrl, 0x00, sizeof(txfctrl));
//         txfctrl.tx_flen = lenght;
//         txfctrl.tx_br = 1;
//         txfctrl.tx_psr = 1;
//         txfctrl.fine_plen = 0;

//         // read back STS pkg format
//         DW3000_getConfig(data_read_sys_cfg, cs_pin);
//         memcpy(&sys_cfg, data_read_sys_cfg, sizeof(data_read_sys_cfg));
//         Serial.print("sys_cfg.CP_SDC: ");
//         Serial.println(sys_cfg.CP_SDC);
//         // set it to SP3
//         if (sys_cfg.CP_SDC != SP3)
//         {
//             sys_cfg.CP_SDC = SP3;
//         }
//         DW3000_setConfig(&sys_cfg, cs_pin);

//         // Then write the transmit frame config
//         DW3000_writeTXFCTRL(&txfctrl);

//         // update CHN_CONFIG
//         memset(&txcctrl, 0x00, sizeof(txcctrl));
//         txcctrl.RF_CHAN = 1;
//         txcctrl.SFD_TYPE = 0x3;
//         txcctrl.TX_PCODE = 0x9;
//         txcctrl.RX_PCODE = 0x9;

//         // Write to Channel control
//         sys_status_controlDW3000_writeCHAN_CTRL(&txcctrl, cs_pin);
//         lRetrunVal = TRUE;
//         break;

//     default:
//         break;
//     }
//     if (lRetrunVal == TRUE)
//     {
//         // Write Fast command for Start transmision section 9.
//         Serial.println("Start TX DW3000");
//         DW3000_startTX(cs_pin);
//         // get time stamp - remember to calib delay later
//         switch (frame)
//         {
//         case PREPOLL:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.Prepoll_timestamp, cs_pin);
//             break;
//         case FINALDATA:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.FinalData_timestamp, cs_pin);
//             break;
//         default:
//             break;
//         }
//         // check if TX complete
//         do
//         {
//             DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data_read_sys_status_control, cs_pin);
//             memcpy(&sys_status_control, data_read_sys_status_control, sizeof(data_read_sys_status_control));
//             Serial.print("sys_status_control.TXFRS: ");
//             Serial.println(sys_status_control.TXFRS);
//             delay(100);
//         } while (sys_status_control.TXFRS != 1);
//         // write back to TXFRS to clear bit
//         tx_msg_control[0] = 0x80;
//         DW3000_writeData(GEN_CFG_AES_1, SYS_STATUS, 1, tx_msg_control, FULL, cs_pin);
//     }
//     else
//     {
//     }
// }

// // transmit SP3 pkg - Poll, Respond and Final (SP3 STS without data)
// void DW3000_Transmitframe_SP3(CCC_FRAME frame, uint8_t lenght, uint8_t offset, uint8_t cs_pin)
// {
//     bool lRetrunVal = FALSE;
//     DW3000_TX_FCTRL txfctrl;
//     DW3000_CHAN_CTRL txcctrl;
//     switch (frame)
//     {
//     case POLL:

//         // we config for this to be a valid PrePoll frame
//         // write our config for sending ranging frame
//         memset(&txfctrl, 0x00, sizeof(txfctrl));
//         txfctrl.tx_flen = 0;
//         txfctrl.tx_br = 1;
//         txfctrl.tx_psr = 1;
//         txfctrl.fine_plen = 0;

//         // read back STS pkg format
//         DW3000_getConfig(data_read_sys_cfg, cs_pin);
//         memcpy(&sys_cfg, data_read_sys_cfg, sizeof(data_read_sys_cfg));
//         Serial.print("sys_cfg.CP_SDC: ");
//         Serial.println(sys_cfg.CP_SDC);
//         // set it to SP3
//         if (sys_cfg.CP_SDC != SP0)
//         {
//             sys_cfg.CP_SDC = SP0;
//         }
//         DW3000_setConfig(&sys_cfg, cs_pin);

//         // Then write the transmit frame config
//         DW3000_writeTXFCTRL(&txfctrl);

//         // update CHN_CONFIG

//         memset(&txcctrl, 0x00, sizeof(txcctrl));
//         txcctrl.RF_CHAN = 1;
//         txcctrl.SFD_TYPE = 0x3;
//         txcctrl.TX_PCODE = 0x9;
//         txcctrl.RX_PCODE = 0x9;

//         // Write to Channel control
//         sys_status_controlDW3000_writeCHAN_CTRL(&txcctrl, cs_pin);
//         lRetrunVal = TRUE;
//         break;

//     case RESPOND:

//         // then we config for this to be a valid PrePoll frame
//         // write our config for sending ranging frame
//         memset(&txfctrl, 0x00, sizeof(txfctrl));
//         txfctrl.tx_flen = 0;
//         txfctrl.tx_br = 1;
//         txfctrl.tx_psr = 1;
//         txfctrl.fine_plen = 0;

//         // read back STS pkg format
//         DW3000_getConfig(data_read_sys_cfg, cs_pin);
//         memcpy(&sys_cfg, data_read_sys_cfg, sizeof(data_read_sys_cfg));
//         Serial.print("sys_cfg.CP_SDC: ");
//         Serial.println(sys_cfg.CP_SDC);
//         // set it to SP3
//         if (sys_cfg.CP_SDC != SP0)
//         {
//             sys_cfg.CP_SDC = SP0;
//         }
//         DW3000_setConfig(&sys_cfg, cs_pin);

//         // Then write the transmit frame config
//         DW3000_writeTXFCTRL(&txfctrl);

//         // update CHN_CONFIG
//         memset(&txcctrl, 0x00, sizeof(txcctrl));
//         txcctrl.RF_CHAN = 1;
//         txcctrl.SFD_TYPE = 0x3;
//         txcctrl.TX_PCODE = 0x9;
//         txcctrl.RX_PCODE = 0x9;

//         // Write to Channel control
//         sys_status_controlDW3000_writeCHAN_CTRL(&txcctrl, cs_pin);
//         lRetrunVal = TRUE;
//         break;
//     case FINAL:
//         // then we config for this to be a valid PrePoll frame
//         // write our config for sending ranging frame
//         memset(&txfctrl, 0x00, sizeof(txfctrl));
//         txfctrl.tx_flen = 0;
//         txfctrl.tx_br = 1;
//         txfctrl.tx_psr = 1;
//         txfctrl.fine_plen = 0;

//         // read back STS pkg format
//         DW3000_getConfig(data_read_sys_cfg, cs_pin);
//         memcpy(&sys_cfg, data_read_sys_cfg, sizeof(data_read_sys_cfg));
//         Serial.print("sys_cfg.CP_SDC: ");
//         Serial.println(sys_cfg.CP_SDC);
//         // set it to SP3
//         if (sys_cfg.CP_SDC != SP0)
//         {
//             sys_cfg.CP_SDC = SP0;
//         }
//         DW3000_setConfig(&sys_cfg, cs_pin);

//         // Then write the transmit frame config
//         DW3000_writeTXFCTRL(&txfctrl);

//         // update CHN_CONFIG
//         memset(&txcctrl, 0x00, sizeof(txcctrl));
//         txcctrl.RF_CHAN = 1;
//         txcctrl.SFD_TYPE = 0x3;
//         txcctrl.TX_PCODE = 0x9;
//         txcctrl.RX_PCODE = 0x9;

//         // Write to Channel control
//         sys_status_controlDW3000_writeCHAN_CTRL(&txcctrl, cs_pin);
//         lRetrunVal = TRUE;
//         break;
//     default:
//         break;
//     }
//     if (lRetrunVal == TRUE)
//     {
//         // Write Fast command for Start transmision section 9.
//         Serial.println("Start TX DW3000");
//         DW3000_startTX(cs_pin);
//         // get time stamp - remember to calib delay later
//         switch (frame)
//         {
//         case PREPOLL:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.Prepoll_timestamp, cs_pin);
//             break;
//         case FINALDATA:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.FinalData_timestamp, cs_pin);
//             break;
//         default:
//             break;
//         }
//         // check if TX complete
//         do
//         {
//             DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data_read_sys_status_control, cs_pin);
//             memcpy(&sys_status_control, data_read_sys_status_control, sizeof(data_read_sys_status_control));
//             Serial.print("sys_status_control.TXFRS: ");
//             Serial.println(sys_status_control.TXFRS);
//             delay(100);
//         } while (sys_status_control.TXFRS != 1);
//         // write back to TXFRS to clear bit
//         tx_msg_control[0] = 0x80;
//         DW3000_writeData(GEN_CFG_AES_1, SYS_STATUS, 1, tx_msg_control, FULL, cs_pin);
//     }
//     else
//     {
//     }
// }

// void DW3000_Receiveframe(CCC_FRAME frame, uint8_t lenght, uint8_t offset, uint8_t cs_pin)
// {
//     bool lRetrunVal = FALSE;
//     DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data_read_sys_status_control, cs_pin);
//     memcpy(&sys_status_control, data_read_sys_status_control, sizeof(data_read_sys_status_control));
//     Serial.print("sys_status_control.RXFR: ");
//     Serial.println(sys_status_control.RXFR);
//     lRetrunVal = sys_status_control.RXFR;
//     if (lRetrunVal == TRUE)
//     {
//         switch (frame)
//         {
//         case PREPOLL:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.Prepoll_timestamp, cs_pin);
//             break;
//         case POLL:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.Poll_timestamp, cs_pin);
//             break;
//         case RESPOND:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.Respond_timestamp, cs_pin);
//             break;
//         case FINAL:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.Final_timestamp, cs_pin);
//             break;
//         case FINALDATA:
//             DW3000_readData(GEN_CFG_AES_1, TX_TIME, TX_TIME_LEN, timestamp.FinalData_timestamp, cs_pin);
//             break;
//         default:
//             break;
//         }
//     }
// }

void DW_3000_Calib(uint8_t cs_pin)
{
    uint8_t temp_data[4];
    uint32_t temp_data32;
    uint32_t temp_data16;

    temp_data16 = 0x0001;
    memcpy(temp_data, &temp_data16, sizeof(temp_data16));
    DW3000_writeData(0x06, 0x00, 0x02, temp_data, FULL, cs_pin);
    temp_data16 = 0x0041;
    memcpy(temp_data, &temp_data16, sizeof(temp_data16));
    DW3000_writeData(0x06, 0x02, 0x02, temp_data, FULL, cs_pin);

    temp_data32 = 0xAF5F584C;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x06, 0x0C, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x10000240;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x1C, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x1b6da489;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x20, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x0002A8FE;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x38, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x0002AC36;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x3C, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x0002A5FE;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x40, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x0002AF3E;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x44, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x0002AF7d;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x48, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x0002AFB5;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x4C, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x0002AFB5;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x03, 0x50, 0x04, temp_data, FULL, cs_pin);

    temp_data16 = 0x3201;
    memcpy(temp_data, &temp_data16, sizeof(temp_data16));
    DW3000_writeData(0x03, 0x18, 0x02, temp_data, FULL, cs_pin);

    temp_data32 = 0x02001C00;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x07, 0x00, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x02001C00;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x07, 0x04, 0x04, temp_data, FULL, cs_pin);

    temp_data[0] = 0x14;
    DW3000_writeData(0x07, 0x48, 0x01, temp_data, FULL, cs_pin);

    temp_data[0] = 0x0E;
    DW3000_writeData(0x07, 0x1A, 0x01, temp_data, FULL, cs_pin);

    temp_data[0] = 0x81;
    DW3000_writeData(0x09, 0x08, 0x01, temp_data, FULL, cs_pin);

    temp_data32 = 0xB40200;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x11, 0x04, 0x03, temp_data, FULL, cs_pin);

    temp_data32 = 0x80030738;
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x11, 0x08, 0x04, temp_data, FULL, cs_pin);

    temp_data32 = 0x1C010034;
    temp_data16 = 0x0F3C;//0x0F3C; channel 9
    memcpy(temp_data, &temp_data32, sizeof(temp_data32));
    DW3000_writeData(0x07, RF_TX_CTRL_2, RF_TX_CTRL_2_LEN, temp_data, FULL, cs_pin);
    memcpy(temp_data, &temp_data16, sizeof(temp_data16));
    DW3000_writeData(0x09, 0x00, 0x02, temp_data, FULL, cs_pin);

    temp_data[0] = 0x01;
    DW3000_writeData(0x08, 0x00, 0x01, temp_data, FULL, cs_pin);
    do
    {
      DW3000_readData(0x08, 0x04, 0x01,temp_data, cs_pin);
    }while((temp_data[0] & 0x01) != 1);

    temp_data16 = 0x0034;
    memcpy(temp_data, &temp_data16, sizeof(temp_data16));
    DW3000_writeData(0x08, 0x1C, 0x02, temp_data, FULL, cs_pin);

    DW3000_readData(0x08, 0x10, 0x01,temp_data, cs_pin);
    temp_data[0] |= 0x03;
    DW3000_writeData(0x08, 0x10, 0x01, temp_data, FULL, cs_pin);

    do
    {
      DW3000_readData(0x08, 0x10, 0x01,temp_data, cs_pin);
    }while((temp_data[0] & 0x03) != 0);


}

void DW3000_CheckIdle(uint8_t cs_pin)
{
    DW3000_SYS_STATE sys_state;
    do
    {
        DW3000_readData(SYS, SYS_STATE, SYS_STATE_LEN, sys_state.data, cs_pin);
    } while (((sys_state.TSE_STATE & IDLE) == 0 ));
    
    
}