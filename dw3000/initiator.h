#include "AU_DW3000.h"
#include "DW3000_Reg_Def.h"

static uint8_t tx_msg_init[] = {0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E'};
static SPI_WRITE_TYPE gwtype_init = SHORT;
static uint8_t data_read_sys_status_init[4] = {0};
static uint8_t data_read_sys_cfg[4] = {0};
static DW3000_SYS_STATUS sys_status_init;
void DW3000_init_channelcfg (uint8_t cs_pin);
typedef enum
{
    PREPOLL,
    POLL,
    RESPOND,
    FINAL,
    FINALDATA
} CCC_FRAME;

typedef struct 
{
    uint8_t Prepoll_timestamp[5];
    uint8_t Poll_timestamp[5];
    uint8_t Respond_timestamp[5];
    uint8_t Final_timestamp[5];
    uint8_t FinalData_timestamp[5];
}initiator_timestamp;


void DW3000_Transmitframe_single(uint8_t cs_pin);

bool DW3000_Initiator_init_sys(uint8_t cs_pin);