#include "initiator.h"


initiator_timestamp timestamp;
void DW3000_Transmitframe_single(uint8_t cs_pin)
{
    uint8_t temp_data[4];
    
    // First write our transmit buffer
    DW3000_TX_FCTRL txfctrl;
        // write our config for sending ranging frame
    memset(&txfctrl, 0x00, sizeof(txfctrl));
    txfctrl.tx_flen = 0x6F;
    txfctrl.tx_br = 1;
    txfctrl.tx_psr = 1;
    txfctrl.fine_plen = 0;        

    // Then write the transmit frame config
    DW3000_writeTXFCTRL(&txfctrl, cs_pin);
    uint8_t buffer_Tx[127] = {0x0D, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    DW3000_writeTXData(10, buffer_Tx, 0);




    // Write Fast command for Start transmision section 9.
    //Serial.println("Start TX DW3000");

    DW3000_startTX(cs_pin);
    DW3000_SYS_STATE sys_state;
    uint8_t buffer_fstcmd[1];
    DW3000_readData(SYS, 0x3C, 1, buffer_fstcmd, cs_pin);
    DW3000_readData(SYS, SYS_STATE, SYS_STATE_LEN, sys_state.data, cs_pin);
    Serial.print("sys_state.TX_STATE ");
    Serial.println(sys_state.TX_STATE);
    Serial.print("sys_state.RX_STATE ");
    Serial.println(sys_state.RX_STATE);
    Serial.print("sys_state.TSE_STATE ");
    Serial.println(sys_state.TSE_STATE);
    Serial.print("buffer_fstcmd ");
    Serial.println(buffer_fstcmd[0]);
    // check if TX complete

    do
    {
        DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data_read_sys_status_init, cs_pin);
        memcpy(&sys_status_init, data_read_sys_status_init, sizeof(data_read_sys_status_init));
        delay(100);
    } while (sys_status_init.TXFRS != 1);
    // write back to TXFRS to clear bit
    tx_msg_init[0] = 0x0F;
    DW3000_writeData(GEN_CFG_AES_1, SYS_STATUS, 1, tx_msg_init, FULL, cs_pin);

    // read again to see if its clear
    DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data_read_sys_status_init, cs_pin);
    // // we will send a msg every 288ms
    // delay(288);
    buffer_Tx[2]++;
    if (buffer_Tx[2] == 0)
    {
        buffer_Tx[3]++;
    }
}

bool DW3000_Initiator_init_sys(uint8_t cs_pin)
{
    uint8_t sys_cfg_read[4] = {0,};
    DW3000_SYS_CFG sys_cfg_init;
    bool lReturnval = FALSE;
    uint8_t temp_check = 0;
    memset(sys_cfg_init.data, 0x00, sizeof(sys_cfg_init.data));
    // enable Frame Filtering for PanId and Short address
    sys_cfg_init.FFEN = 0;
    /*disable receving 2 buffer at the same time*/
    sys_cfg_init.DIS_DRXB = 1;
    /* use 6.8mHZ transfer*/
    sys_cfg_init.PHR_6M8 = 1;
    /*secure ranging*/
    sys_cfg_init.CIA_IPATOV = 1;
    /* byte 2*/
    sys_cfg_init.CIA_STS = 1;
    /* byte 3 */
    /*enalbe PDOA mode 3 have some specific STS lenght*/
    sys_cfg_init.PDOA_MODE = 0;
    sys_cfg_init.SPI_CRCEN = 0;

uint8_t temp_data[4];
  uint16_t temp_data16;
    temp_data[0] = 0x04;
    DW3000_writeData(0x0b, 0x04, 0x01, temp_data, FULL, cs_pin);
    temp_data[0] = 0x02;
    DW3000_writeData(0x0b, 0x08, 0x01, temp_data, FULL, cs_pin);
    uint32_t ldo_low;
    DW3000_readData(0x08, 0x10, 0x01,ldo_low, cs_pin);

    temp_data[0] = 0x05;
    DW3000_writeData(0x0b, 0x04, 0x01, temp_data, FULL, cs_pin);
    temp_data[0] = 0x02;
    DW3000_writeData(0x0b, 0x08, 0x01, temp_data, FULL, cs_pin);
    uint32_t ldo_high;
    DW3000_readData(0x08, 0x10, 0x01,ldo_high, cs_pin);

    temp_data[0] = 0xA;
    DW3000_writeData(0x0b, 0x04, 0x01, temp_data, FULL, cs_pin);
    temp_data[0] = 0x02;
    DW3000_writeData(0x0b, 0x08, 0x01, temp_data, FULL, cs_pin);
    uint32_t bias;
    DW3000_readData(0x08, 0x10, 0x01,bias, cs_pin);

    bias = (bias >> 16) ^0x1f;
    if (ldo_low != 0 && ldo_high != 0 && bias != 0) {
      memcpy(temp_data, &bias, sizeof(bias));
      DW3000_writeData(0x11, 0x1f, 0x04, temp_data, FULL, cs_pin);

      temp_data16 = 0x0100;
      memcpy(temp_data, &temp_data16, sizeof(temp_data16));

       DW3000_writeData(0x0B, 0x08, 0x02, temp_data, FULL, cs_pin);
    }

  temp_data[0] = 0x1E;
  DW3000_writeData(0x0b, 0x04,0x01, temp_data, FULL, cs_pin);
  temp_data[0] = 0x02;
  DW3000_writeData(0x0b, 0x08,0x01, temp_data, FULL, cs_pin);
  int trim;
  DW3000_readData(0x08, 0x10, 0x01,trim, cs_pin);

  trim = trim == 0 ? 0x2E : trim;
  memcpy(temp_data, &trim, sizeof(trim));
      DW3000_writeData(0x09, 0x14, 0x04, temp_data, FULL, cs_pin);

    temp_check = sys_cfg_init.data[0];
    DW3000_init(&sys_cfg_init, cs_pin);
    DW3000_getConfig(sys_cfg_read, cs_pin);
    lReturnval = (sys_cfg_read[0] == (temp_check & 0xFF)) ? TRUE : FALSE;
    uint32_t temp = 0x00300200;
    memcpy(sys_cfg_read,&temp,sizeof(temp));
    DW3000_writeData(0x11, 0x04, 4, sys_cfg_read, FULL, cs_pin);
    temp = 0x00000138;
    memcpy(sys_cfg_read,&temp,sizeof(temp));
    DW3000_writeData(0x11, 0x08, 2, sys_cfg_read, FULL, cs_pin);
    temp = 0xFDFDFDFD;
    memcpy(sys_cfg_read,&temp,sizeof(temp));
    DW3000_writeData(0x01, 0x0C,0x04, 0xFDFDFDFD, FULL, cs_pin);
    return lReturnval;
}
void DW3000_init_channelcfg (uint8_t cs_pin)
{
      // update CHN_CONFIG

    DW3000_CHAN_CTRL txcctrl;
    memset(txcctrl.data, 0x00, sizeof(txcctrl.data));
    txcctrl.RF_CHAN = 1;
    txcctrl.SFD_TYPE = 0x3;
    txcctrl.TX_PCODE = 0x9;
    txcctrl.RX_PCODE = 0x9;

    // Write to Channel control
    DW3000_writeData(GEN_CFG_AES_2, CHAN_CTRL, CHAN_CTRL_LEN, txcctrl.data, FULL, cs_pin);
    
}