#include "AU_DW3000.h"
#include "responder.h"
#include "initiator.h"
#include "control.h"
uint8_t sys_cfg_read[4] = {0,};

extern DW3000_SYS_CFG sys_cfg = {};
//#define RUN_TX_MODE
#define RUN_RX_MODE
static bool gInitcomplete = FALSE;

void setup() {
  uint8_t temp_data_1[4];
  bool lReturnval = FALSE;
  uint8_t temp_check = 0;
  Serial.begin(9600);
  //SPI init
  DW3000_Spi_init();
  //RSTn init
  DW3000_RST_init();

  delay(1000);
  #ifdef RUN_TX_MODE
  DW3000_CheckIdle(DEFAULT_CS);
  

  // Read device ID
  lReturnval = DW3000_getDevID(DEFAULT_CS);
  DW3000_getConfig(sys_cfg_read, DEFAULT_CS);
  // config DW3000
  if(lReturnval == TRUE)
  {
    memset(sys_cfg_read,0xff,sizeof(sys_cfg_read));
    DW3000_writeData(GEN_CFG_AES_1, SYS_STATUS, 4, sys_cfg_read,FULL, DEFAULT_CS);
     Serial.println("syeah 1 ");
     
     temp_data_1[0] = 0x05;
     temp_data_1[1] = 0x01;
    DW3000_writeData(0x07, 0x48, 0x02, temp_data_1, FULL, DEFAULT_CS);
    lReturnval = DW3000_Initiator_init_sys(DEFAULT_CS);
    
    DW3000_init_channelcfg(DEFAULT_CS);
    DW_3000_Calib(DEFAULT_CS);
    
  } 
  if(lReturnval != TRUE)
  {
    Serial.println("DW3000_init DEFAULT_CS failed");
  }
  DW3000_CheckIdle(DEFAULT_CS);
  #endif

  #ifdef RUN_RX_MODE
  DW3000_CheckIdle(DEFAULT_CS);
  // init the second one
  lReturnval = DW3000_getDevID(DEFAULT_CS);
  // config DW3000
  
  
  if(lReturnval == TRUE)
  {
    memset(sys_cfg_read,0xff,sizeof(sys_cfg_read));
    DW3000_writeData(GEN_CFG_AES_1, SYS_STATUS, 4, sys_cfg_read,FULL, DEFAULT_CS);
    Serial.println("CALIB TIME ");
    temp_data_1[0] = 0x05;
     temp_data_1[1] = 0x01;
    DW3000_writeData(0x07, 0x48, 0x02, temp_data_1, FULL, DEFAULT_CS);
    lReturnval = DW3000_Responder_init_sys(DEFAULT_CS);
  } 
  Serial.println("CALIB DONE ");
  if(lReturnval == TRUE)
  {
    Serial.println("syeah 3 ");
    Serial.println("DW3000_init Pass");
    gInitcomplete = TRUE;
  }
  // DW3000_CheckIdle(DEFAULT_CS);
      DW3000_SYS_STATE sys_state;
    do
    {
        DW3000_readData(SYS, SYS_STATE, SYS_STATE_LEN, sys_state.data, DEFAULT_CS);
        Serial.println(sys_state.TSE_STATE);
    } while (((sys_state.TSE_STATE & IDLE) != 0x03 ));

    DW3000_Responder_init_Channel(DEFAULT_CS);
    DW3000_RX_CAL RX_caldata;
    DW3000_RX_CAL_RESI Rx_calib_resi;
    DW3000_RX_CAL_RESQ Rx_calib_resq;
    do
    {
      RX_caldata.COMP_DLY = 2;
      RX_caldata.CAL_EN = 1;
      RX_caldata.CAL_MODE = 1;

      DW3000_writeData(EXT_SYNC, RX_CAL, RX_CAL_LEN, RX_caldata.data, FULL, DEFAULT_CS);
      do
      {
        DW3000_readData(EXT_SYNC, RX_CAL, RX_CAL_LEN, RX_caldata.data, DEFAULT_CS);
      } while (RX_caldata.CAL_EN == 1);
      RX_caldata.CAL_MODE = 0;
      DW3000_writeData(EXT_SYNC, RX_CAL, RX_CAL_LEN, RX_caldata.data, FULL, DEFAULT_CS);
      DW_3000_Calib(DEFAULT_CS);
      
      DW3000_readData(EXT_SYNC, RX_CAL, RX_CAL_LEN, Rx_calib_resq.data, DEFAULT_CS);
      DW3000_readData(EXT_SYNC, RX_CAL, RX_CAL_LEN, Rx_calib_resi.data, DEFAULT_CS);
    }while((Rx_calib_resi.rx_cal_resi == 0x1fffffff) ||(Rx_calib_resq.rx_cal_resq == 0x1fffffff));
  #endif
  DW3000_SYS_CFG sys_cfg_read1;
  DW3000_readData(0x11, 0x08, 2, sys_cfg_read1.data , DEFAULT_CS);
  Serial.println( sys_cfg_read1.FFEN);
  Serial.println( sys_cfg_read1.PHR_MODE);
  Serial.println( sys_cfg_read1.PDOA_MODE);
  Serial.println( sys_cfg_read1.PHR_6M8);
  delay(3000);
  
}

void loop()
{
 // enable Frame Filtering for PanId and Short address
 
  uint8_t read_sys_buffer[4];
  #ifdef RUN_RX_MODE
   if (gInitcomplete == TRUE)
   {

    DW3000_startRX(DEFAULT_CS);
    // DW_3000_Calib(DEFAULT_CS);
    // DW_3000_Calib(DEFAULT_CS);
  // // for testing purpose open RX window imidiately with fast cmd CMD_RX
    

  Serial.println("syeah 4 ");
    gInitcomplete = FALSE;
   }
    
  do
  {
 
    DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 6, data_read_sys_status_respond, DEFAULT_CS);
    memcpy(&sys_status_respond, data_read_sys_status_respond, sizeof(data_read_sys_status_respond));
     delay(1000);
    memcpy(read_sys_buffer,&sys_status_respond,sizeof(read_sys_buffer));
    Serial.print("sys_status_respond byte[0] ");
    Serial.println(read_sys_buffer[0]);
    Serial.print("sys_status_respond byte[1] ");
    Serial.println(read_sys_buffer[1]);
    Serial.print("sys_status_respond byte[2] ");
    Serial.println(read_sys_buffer[2]);
    Serial.print("sys_status_respond byte[3] ");
    Serial.println(read_sys_buffer[3]);
        Serial.print("sys_status_respond byte[4] ");
    Serial.println(read_sys_buffer[4]);
    Serial.print("sys_status_respond byte[5] ");
    Serial.println(read_sys_buffer[5]);
    Serial.println("++++++++++++++++++++++++ ");
    DW3000_RX_FINFO rx_info;
    DW3000_readData(0x00, 0x4C, 4, rx_info.data, DEFAULT_CS);
    Serial.print("rx_info.data byte[0] ");
    Serial.println(rx_info.data[0]);
    Serial.print("rx_info.data byte[1] ");
    Serial.println(rx_info.data[1]);
    Serial.print("rx_info.data byte[2] ");
    Serial.println(rx_info.data[2]);
    Serial.print("rx_info.data byte[3] ");
    Serial.println(rx_info.data[3]);
    Serial.println("++++++++++++++++++++++++ ");
        DW3000_SYS_STATE sys_state;
    // do
    // {
    uint8_t buffer_fstcmd[1];
    DW3000_readData(SYS, 0x3C, 1, buffer_fstcmd, DEFAULT_CS);
    DW3000_readData(SYS, SYS_STATE, SYS_STATE_LEN, sys_state.data, DEFAULT_CS);
    Serial.print("sys_state.TX_STATE ");
    Serial.println(sys_state.TX_STATE);
    Serial.print("sys_state.RX_STATE ");
    Serial.println(sys_state.RX_STATE);
    Serial.print("sys_state.TSE_STATE ");
    Serial.println(sys_state.TSE_STATE);
    Serial.print("buffer_fstcmd ");
    Serial.println(buffer_fstcmd[0]);
    // } while (((sys_state.TSE_STATE & IDLE) == 0 ));
  } while (1);

  #endif

  #ifdef RUN_TX_MODE
  // uint8_t data_read_sys_status_respond[4]; 
    DW3000_Transmitframe_single(DEFAULT_CS);
    delay(1000);
    DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data_read_sys_status_respond, DEFAULT_CS);
    memcpy(&sys_status_respond, data_read_sys_status_respond, sizeof(data_read_sys_status_respond));
    memcpy(read_sys_buffer,&sys_status_respond,sizeof(read_sys_buffer));
    Serial.print("sys_status_respond byte[0] ");
    Serial.println(read_sys_buffer[0]);
    Serial.print("sys_status_respond byte[1] ");
    Serial.println(read_sys_buffer[1]);
    Serial.print("sys_status_respond byte[2] ");
    Serial.println(read_sys_buffer[2]);
    Serial.print("sys_status_respond byte[3] ");
    Serial.println(read_sys_buffer[3]);

  #endif
  //   // DW3000_Receiveframe_single();

  // // // we will send a msg every 288ms
  //  delay(100);
}

