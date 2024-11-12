#include "responder.h"

void DW3000_Receiveframe_single()
{

  uint8_t buffer_Rx[127] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t buffer_Rx_info[4] = {0x00, 0x00, 0x00, 0x00};
  // update CHN_CONFIG
  // DW3000_CHAN_CTRL txcctrl;
  // memset(&txcctrl, 0x00, sizeof(txcctrl));
  // txcctrl.RF_CHAN = 1;
  // txcctrl.SFD_TYPE = 0x3;
  // txcctrl.TX_PCODE = 0x9;
  // txcctrl.RX_PCODE = 0x9;

  // // Write to Channel control
  // DW3000_writeCHAN_CTRL(&txcctrl,MOD_CS);
  // // for testing purpose open RX window imidiately with fast cmd CMD_RX
  // DW3000_startRX(MOD_CS);
  DW3000_readData(GEN_CFG_AES_1, RX_FINFO, RX_FINFO_LEN, buffer_Rx_info, MOD_CS);
  // check if RX receive
  do
  {
    DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data_read_sys_status_respond, MOD_CS);
    memcpy(&sys_status_respond, data_read_sys_status_respond, sizeof(data_read_sys_status_respond));
    Serial.print("sys_status_respond.RXFR: ");
    Serial.println(sys_status_respond.RXFCG);
  } while (sys_status_respond.RXFR != 1);
  if (sys_status_respond.RXFR == 1)
  {
    Serial.println("receive success");
    tx_msg_resp[0] = 0x00;
    tx_msg_resp[1] = 0x20;
    // write 1 to clear bit
    DW3000_writeData(GEN_CFG_AES_1, SYS_STATUS, 2, tx_msg_resp, FULL, MOD_CS);

    // read receive frame info
    DW3000_readData(GEN_CFG_AES_1, RX_FINFO, RX_FINFO_LEN, buffer_Rx_info, MOD_CS);
    memcpy(&rx_info, buffer_Rx_info, sizeof(buffer_Rx_info));
    // if we got the correct lenght later we will check for data correctness
    if (uint8_t(rx_info.RXFLEN) == 10)
    {
      // data lenght minus for 2 to get the correct lenght
      DW3000_getRXData(125, buffer_Rx, 0, MOD_CS);
      if ((buffer_Rx[0] == 0x0D, 0xA0) && (buffer_Rx[0] == 0xA0))
      {
        Serial.println("receive success");
      }
    }
  }
}

void DW3000_Responder_init_Channel(uint8_t cs_pin)
{
  DW3000_CHAN_CTRL txcctrl;
  memset(txcctrl.data, 0x00, sizeof(txcctrl.data));
  txcctrl.RF_CHAN = 1;
  txcctrl.SFD_TYPE = 0x3;
  txcctrl.TX_PCODE = 0x9;
  txcctrl.RX_PCODE = 0x9;

  // Write to Channel contro
  DW3000_writeData(GEN_CFG_AES_2, CHAN_CTRL, CHAN_CTRL_LEN, txcctrl.data, FULL, cs_pin);
}

bool DW3000_Responder_init_sys(uint8_t cs_pin)
{
  uint8_t sys_cfg_read[4] = {0,};
  uint8_t temp_check = 0;
  DW3000_SYS_CFG sys_cfg_resp;
  bool lReturnval = FALSE;
  uint8_t temp_data[4] ;
  uint16_t temp_data16;
    // temp_data[0] = 0x04;
    // DW3000_writeData(0x0b, 0x04, 0x01, temp_data, FULL, cs_pin);
    // temp_data[0] = 0x02;
    // DW3000_writeData(0x0b, 0x08, 0x01, temp_data, FULL, cs_pin);
    // uint32_t ldo_low;
    // DW3000_readData(0x08, 0x10, 0x01,ldo_low, cs_pin);

    // temp_data[0] = 0x05;
    // DW3000_writeData(0x0b, 0x04, 0x01, temp_data, FULL, cs_pin);
    // temp_data[0] = 0x02;
    // DW3000_writeData(0x0b, 0x08, 0x01, temp_data, FULL, cs_pin);
    // uint32_t ldo_high;
    // DW3000_readData(0x08, 0x10, 0x01,ldo_high, cs_pin);

    // temp_data[0] = 0xA;
    // DW3000_writeData(0x0b, 0x04, 0x01, temp_data, FULL, cs_pin);
    // temp_data[0] = 0x02;
    // DW3000_writeData(0x0b, 0x08, 0x01, temp_data, FULL, cs_pin);
    // uint32_t bias;
    // DW3000_readData(0x08, 0x10, 0x01,bias, cs_pin);

    // bias = (bias >> 16) ^0x1f;
    // if (ldo_low != 0 && ldo_high != 0 && bias != 0) {
    //   memcpy(temp_data, &bias, sizeof(bias));
    //   DW3000_writeData(0x11, 0x1f, 0x04, temp_data, FULL, cs_pin);

    //   temp_data16 = 0x0100;
    //   memcpy(temp_data, &temp_data16, sizeof(temp_data16));

    //    DW3000_writeData(0x0B, 0x08, 0x02, temp_data, FULL, cs_pin);
    // }

  // temp_data[0] = 0x1E;
  // DW3000_writeData(0x0b, 0x04,0x01, temp_data, FULL, cs_pin);
  // temp_data[0] = 0x02;
  // DW3000_writeData(0x0b, 0x08,0x01, temp_data, FULL, cs_pin);
  // int trim;
  // DW3000_readData(0x08, 0x10, 0x01,trim, cs_pin);

  // trim = trim == 0 ? 0x2E : trim;
  // memcpy(temp_data, &trim, sizeof(trim));
  //     DW3000_writeData(0x09, 0x14, 0x04, temp_data, FULL, cs_pin);

  memset(sys_cfg_resp.data, 0x00, sizeof(sys_cfg_resp.data));
  // enable Frame Filtering for PanId and Short address
  sys_cfg_resp.FFEN = 0;
  /*disable receving 2 buffer at the same time*/
  sys_cfg_resp.DIS_DRXB = 1;
  /* use 6.8mHZ transfer*/
  sys_cfg_resp.PHR_6M8 = 1;
  /*secure ranging*/
  sys_cfg_resp.CIA_IPATOV = 1;
  /* byte 2*/
  sys_cfg_resp.CIA_STS = 1;
  /* byte 3 */
  sys_cfg_resp.DIS_FCE = 1;
  sys_cfg_resp.DIS_FCS_TX = 1;
  /*enalbe PDOA mode 3 have some specific STS lenght*/
  sys_cfg_resp.PDOA_MODE = 0;
  sys_cfg_resp.SPI_CRCEN = 0;
  // sys_cfg_resp.RXAUTR = 1;
  //  sys_cfg_resp.RXWTOE = 1;
  temp_check = sys_cfg_resp.data[0];
  DW3000_init(&sys_cfg_resp, cs_pin);
  DW3000_getConfig(sys_cfg_read, cs_pin);
  lReturnval = (sys_cfg_read[0] == (temp_check & 0xFF)) ? TRUE : FALSE;
  uint32_t temp = 0x300200;
  memcpy(sys_cfg_read,&temp,sizeof(temp));
  DW3000_writeData(0x11, 0x04, 4, sys_cfg_read, FULL, cs_pin);
  temp = 0x0138;
  memcpy(sys_cfg_read,&temp,sizeof(temp));
  DW3000_writeData(0x11, 0x08, 2, sys_cfg_read, FULL, cs_pin);
  
  memset(sys_cfg_read,0xff,sizeof(sys_cfg_read));
  DW3000_writeData(0x00, 0x3C, 0x04, sys_cfg_read, FULL, cs_pin);
  DW3000_writeData(0x00, 0x40, 0x02, sys_cfg_read, FULL, cs_pin);
  temp = 0x000900;
  memcpy(sys_cfg_read,&temp,sizeof(temp));
  DW3000_writeData(0x0A, 0x00, 0x03, sys_cfg_read, FULL, cs_pin);
  return lReturnval;
}

