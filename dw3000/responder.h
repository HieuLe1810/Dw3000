#include "AU_DW3000.h"
#include "DW3000_Reg_Def.h"

static uint8_t tx_msg_resp[] = {0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E'};
static uint8_t rx_buffer[127] = {0,};
static SPI_WRITE_TYPE gwtype_resp = SHORT;
static DW3000_RX_INFO  rx_info;
static uint8_t data_read_sys_status_respond[6] = {0};
static DW3000_SYS_STATUS sys_status_respond;

void DW3000_Receiveframe_single();
void DW3000_Responder_init_Channel(uint8_t cs_pin);
bool DW3000_Responder_init_sys(uint8_t cs_pin);