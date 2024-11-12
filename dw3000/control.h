#include "AU_DW3000.h"
#include "DW3000_Reg_Def.h"
#define IDLE 0x03
// void DW3000_Transmitframe_SP0 (CCC_FRAME frame, uint8_t *data, uint8_t lenght, uint8_t offset);
// void DW3000_Transmitframe_SP3(CCC_FRAME frame, uint8_t lenght, uint8_t offset);
static uint8_t tx_msg_control[] = {0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E'};
static uint8_t data_read_sys_status_control[4] = {0};
static DW3000_SYS_STATUS sys_status_control;
void DW_3000_Calib(uint8_t cs_pin);
void DW3000_CheckIdle(uint8_t cs_pin);