#include <Arduino.h>
#include "DW3000_Reg_Def.h"
#ifndef AU_DW3000_H
#define AU_DW3000_H

// Arduino Uno - DWM3000 Shield Pin Map
#define SPI_CLK 13u
#define SPI_MISO 12u
#define SPI_MOSI 11u
#define SPI_CSn_Default 10u
#define SPI_CSn_Mod 4u
#define IRQ 8u
#define RSTn 7u

#define LED_RX 4u
#define LED_TX 3u

#define SPI_POL 1u // Arduino <- TX
#define SPI_PHA 0u // Arduino -> RX

// DW3000
#ifndef FALSE
#define FALSE 0u
#endif

#ifndef TRUE
#define TRUE 1u
#endif

#define JUNK 0x00u

// FAST
#define FAST_CMD_TXRXOFF 0x0
#define FAST_CMD_TX 0x1
#define FAST_CMD_RX 0x2

// Register
#define GEN_CFG_AES_1 0x00
#define GEN_CFG_AES_2 0x01
#define RX_BUFFER_0 0x12
#define RX_BUFFER_1 0x13
#define TX_BUFFER 0x14
#define EXT_SYNC 0x04
#define TX_RF 0x07
#define SYS 0x0f

// SPI Transaction Mask
#define READ 0x00
#define WRITE 0x01
#define FAST_CMD 0x80

#define ADDR_8 0x00
#define ADDR_16 0x02

#define ADDR_MASK1 0x80
#define ADDR_MASK2 0x3F

#define M1 0x4000
#define M0 0x8000

// SYS_CFG
// #define FFEN 0x01
// #define DIS_FCS_TX 0x02 // if set this bit wont cal CRC16
// #define DIS_FCE 0x04    // check for errorhandling
// #define DIS_DRXB 0x08   // have 2 buffer at the same time ? maybe later now just disable it
// #define PHR_MODE 0x10   // if 1 extended PHR mode 0 standard PHR mode
// #define PHR_6M8 0x20    // if 1 used 6,8Mhz as data rate if 0 850kb
// #define SPI_CRCEN 0x40  // enable CRC for SPI .... maybe later
// #define CIA_IPATOV 0x80 // enable estimate on CIR for secure ranging

// #define CIA_STS 0x01    // same for STS
// #define RXWTOE 0x02     // preamble timeout ... maybe later
// #define RXAUTR 0x04     // for continousmode ..... maybe later
// #define AUTO_ACK 0x08   // currrently no need
// #define CP_SPC_SP1 0x10 // currrently no need
// #define CP_SPC_SP2 0x20 // currrently no need
// #define CP_SPC_SP3 0x30 // currrently no need
// #define CP_SDC 0x80     // currrently no need

// #define PDOA_MODE_1 0x01 // calculate of phase of arrival from 2 antenna ..... maybe later ?
// #define PDOA_MODE_2 0x02 // calculate of phase of arrival from 2 antenna ..... maybe later ?
// #define PDOA_MODE_3 0x03 // calculate of phase of arrival from 2 antenna ..... maybe later ?
// #define FAST_AAT 0x04


typedef unsigned char uint8_t;
// typedef unsigned short uint16_t;
// typedef unsigned int uint32_t;
// typedef uint8_t bool;

#define SYS_STATE 0x30
#define SYS_STATE_LEN 0x04
// DW3000 Config
typedef enum
{
  FAST,
  SHORT,
  FULL,
  MASK_01,
  MASK_10,
  MASK_11,
} SPI_WRITE_TYPE;

typedef enum
{
  DEFAULT_CS,
  MOD_CS
} SPI_CS;
typedef enum
{
  SP0,
  SP1,
  SP2,
  SP3,
} SP_PKG_TYPE;
typedef union
{
  uint8_t data[4];
   struct 
  {
    uint8_t CAL_MODE:2; // 0 normal 1 cal
    uint8_t reserve1:1;
    uint8_t reserve2:1;
    uint8_t CAL_EN:1;
    uint16_t reserve3:11;
    uint16_t COMP_DLY:4;
  };
}DW3000_RX_CAL;


typedef struct
{
  uint16_t tx_flen : 10;    // 10bit, payload length
  uint8_t tx_br : 1;        // 1bit, bit rate (0: 850Kb/s, 1: 6.81Mb/s)
  uint8_t tr : 1;           // 1bit, 0 doesnt effect anything
  uint8_t tx_psr : 4;       // 4bit, PDF pp. 86 premble symbol
  uint16_t txb_offset : 10; // 10bit, 0(zero ranging)
  uint16_t reserved : 14;   // 14bit reserve
  uint8_t fine_plen : 8;    // 8bit, 0
} DW3000_TX_FCTRL;

typedef union
{
  uint8_t data[4];
  struct 
  {
    uint32_t rx_cal_resi;
  };
}DW3000_RX_CAL_RESI;

typedef union
{
  uint8_t data[4];
  struct 
  {
    uint32_t rx_cal_resq;
  };
}DW3000_RX_CAL_RESQ;
typedef union
{
  uint8_t data[4];
  struct 
  {
     uint8_t RF_CHAN : 1;  // 1bit, Radar channel (0: channel 5, 1: channel 9)
    uint8_t SFD_TYPE : 2; // 2bit, define SFD sequence should define 0x11
    uint8_t TX_PCODE : 5; // 5bit, TX Preamble code select preamble code 9
    uint8_t RX_PCODE : 5; // 5bit, RX Preamble code select preamble code 9 64Mhz
  };
} DW3000_CHAN_CTRL;

typedef struct
{
  uint8_t IRQS : 1;    // 1bit, interrupt request status anyunmask IRQ is set this bit will set otherwise 0
  uint8_t CPLOCK : 1;  // 1bit, clock pll lock status 1: lock 0: unlock
  uint8_t SPICRCE : 1; // check if SPI CRC are correct (need to open CRC mode)
  uint8_t AAT : 1;     // auto ACK ignore
  uint8_t TXFRB : 1;   // check if Tx start ?
  uint8_t TXPRS : 1;   // check if premble sent ?
  uint8_t TXPHS : 1;   // check if PHY sent?
  uint8_t TXFRS : 1;   // chcek if transtmit done ?
  uint8_t RXPRD : 1;   // check if rx prd receive?
  uint8_t RXSFDD : 1;  // check if rx FDD receive?
  uint8_t CIADONE : 1; // ignore in the mean time
  uint8_t RXPHD : 1;   // ignore
  uint8_t RXPHE : 1;   // ignore
  uint8_t RXFR : 1;    // check if receive msg
  uint8_t RXFCG : 1;   // CRC checking currrently not use
  uint8_t RXFCE : 1;   // check errror
  uint8_t RXFSL : 1;   // solomon error
  uint8_t RXFTO : 1;   // timeout error
  uint8_t CIAERR : 1;  // CIR error
  uint8_t VWARN : 1;   // low voltage
  uint8_t RXOVRR : 1;  // double buffer overun not used currently
  uint8_t RXPTO : 1;   // preamble time out
  uint8_t reserve1 : 1;
  uint8_t SPIRDY : 1;   // SPI ready
  uint8_t RCINIT : 1;   // chipready
  uint8_t PLL_HILO : 1; // PLL losing clock should not happend
  uint8_t RXSTO : 1;    // receive SFD timeout
  uint8_t HPDWARN : 1;
  uint8_t CPERR : 1;    // STS error
  uint8_t ARFE : 1;     // frame fillter reject
  uint8_t RXPREJ : 1;   // receive preamble reject
  uint8_t VT_DET : 1;   // undervoltage
  uint8_t GPIOIRQ : 1;  // GPIO interrupt
  uint8_t AES_DONE : 1; // DMA done
  uint8_t AES_ERR : 1;  // DMA error
  uint8_t CMD_ERR : 1;  // command error
  uint8_t SPI_OVF : 1;  // spi overflow
  uint8_t SPI_UDF : 1;  // spi underflow
  uint8_t SPIERR : 1;   // spi error
  uint8_t CCA_FAIL : 1; // CCA fail to transmit package
} DW3000_SYS_STATUS;

typedef struct 

{
  uint16_t RXFLEN : 10; // 10 bit use for datalenght
  uint8_t reserve1 : 1;
  uint8_t RXNSPL : 2; // 2 bit use for receive but also use for TXSPL (TXPSR = RXNSPL << 2 + RXNSPL).
  uint8_t RXBR : 1;   // 1 bit rate report bit rate (0: 850Kb/s, 1: 6.81Mb/s)
  uint8_t reserve2 : 1;
  uint8_t RNG : 1;     // 1 bit report if a good PHY is receive (when the RXPHD status bit is set)
  uint8_t RXPRF : 2;   // 2 bit use for RX_PCODE programmed in CHAN_CTRL register. The values are: 01 = 16 MHz, 10 = 64 MHz
  uint8_t RXPSR : 2;   // 2 bit use for premble symbol currently we use 64
  uint8_t RXPACC : 11; // dont care yet

} DW3000_RX_INFO;

typedef union 
{
  uint8_t data[SYS_CFG_LEN];
  struct 
  {
    uint8_t FFEN: 1;
    uint8_t DIS_FCS_TX :1;
    uint8_t DIS_FCE :1;
    uint8_t DIS_DRXB :1;
    uint8_t PHR_MODE :1;
    uint8_t PHR_6M8 :1;
    uint8_t SPI_CRCEN :1;
    uint8_t CIA_IPATOV :1;
    uint8_t CIA_STS :1;
    uint8_t RXWTOE :1;
    uint8_t RXAUTR :1;
    uint8_t AUTO_ACK :1;
    uint8_t CP_SPC_SP1 :1;
    uint8_t CP_SPC_SP2 :1;
    uint8_t reserve1 :1;
    uint8_t CP_SDC :1;
    uint8_t PDOA_MODE :2;
    uint8_t FAST_AAT :1;
  };
  

}DW3000_SYS_CFG;

typedef union
{
  uint8_t data[4];
  struct 
  {
    uint16_t RXFLEN: 10;
    uint8_t RXNSPL: 2;
    uint8_t RXBR: 1;
    uint8_t reserve: 1;
    uint8_t RNG: 1;
    uint8_t RXPRF: 1;
    uint8_t RXPSR: 1;
    uint16_t RXPACC: 12;
  };
} DW3000_RX_FINFO;
typedef union
{
  uint8_t data[4];
  struct 
  {
    uint8_t TX_STATE: 4;
    uint8_t reserve1: 4;
    uint8_t RX_STATE: 6;
    uint8_t reserve2: 2;
    uint8_t TSE_STATE: 5;
    uint8_t reserve3: 3;
  };
  
}DW3000_SYS_STATE;

void _readSPI(uint8_t headerLength, uint8_t *headerBuffer, uint8_t readLength, uint8_t *readBuffer, uint8_t cs_pin);
void DW3000_readData(uint8_t reg, uint8_t offset, uint8_t len, uint8_t *data, uint8_t cs_pin);
void DW3000_writeData(uint8_t reg, uint8_t offset, uint8_t len, uint8_t *data, SPI_WRITE_TYPE wtype, uint8_t cs_pin);

void DW3000_setConfig(DW3000_SYS_CFG *sys_cfg, uint8_t cs_pin);
void DW3000_getConfig(uint8_t *sys_cfg, uint8_t cs_pin);
void DW3000_init(DW3000_SYS_CFG *sys_cfg, uint8_t cs_pin);
void DW3000_Spi_init();
void DW3000_RST_init();

bool DW3000_getDevID(uint8_t cs_pin);
void DW3000_getEUI(uint8_t cs_pin);
void DW3000_getOUI(uint8_t cs_pin);
void DW3000_clearIDLE(uint8_t cs_pin);
void DW3000_startTX(uint8_t cs_pin);
void DW3000_startRX(uint8_t cs_pin);
void DW3000_writeCHAN_CTRL(uint8_t *txcctrl, uint8_t cs_pin);
void DW3000_writeTXFCTRL(DW3000_TX_FCTRL *txfctrl, uint8_t cs_pin);
void DW3000_writeTXData(uint16_t len, uint8_t *data, uint16_t bufferOffset);
uint32_t DW3000_getSysStatus(uint8_t cs_pin);
void DW3000_setSysStatus(uint32_t data);
uint16_t DW3000_getRXFrameLength();
void DW3000_getRXData(uint16_t len, uint8_t *data, uint16_t bufferOffset, uint8_t cs_pin);

#endif