#include "AU_DW3000.h"
#include <SPI.h>

const SPISettings _currentSPI = SPISettings(16000000L, LSBFIRST, SPI_MODE0);

uint8_t _reverseBit(uint8_t b)
{
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}
void _writeSPI(uint8_t headerLength, uint8_t *headerBuffer, uint8_t bodyLength, uint8_t *bodyBuffer, uint8_t cs_pin)
{
  SPI.beginTransaction(_currentSPI);
  switch (cs_pin)
  {
    case DEFAULT_CS:
      digitalWrite(SPI_CSn_Default, LOW);
      break;
    case MOD_CS:
      digitalWrite(SPI_CSn_Mod, LOW);
      break;
    default:
      break;
  }
  for (int i = 0; i < headerLength; i++)
  {
    SPI.transfer(headerBuffer[i]); // send header
  }
  for (int i = 0; i < bodyLength; i++)
  {
    SPI.transfer(bodyBuffer[i]); // write values
  }
  delayMicroseconds(5);
  switch (cs_pin)
  {
    case DEFAULT_CS:
      digitalWrite(SPI_CSn_Default, HIGH);
      break;
    case MOD_CS:
      digitalWrite(SPI_CSn_Mod, HIGH);
      break;
    default:
      break;
  }
  SPI.endTransaction();
}

void _readSPI(uint8_t headerLength, uint8_t *headerBuffer, uint8_t readLength, uint8_t *readBuffer, uint8_t cs_pin)
{
  SPI.beginTransaction(_currentSPI);
  switch (cs_pin)
  {
    case DEFAULT_CS:
      digitalWrite(SPI_CSn_Default, LOW);
      break;
    case MOD_CS:
      digitalWrite(SPI_CSn_Mod, LOW);
      break;
    default:
      break;
  }
  for (int i = 0; i < headerLength; i++)
  {
    SPI.transfer(headerBuffer[i]); // send header
  }
  for (int i = 0; i < readLength; i++)
  {
    readBuffer[i] = _reverseBit(SPI.transfer(JUNK)); // read values
  }
  delayMicroseconds(5);
  switch (cs_pin)
  {
    case DEFAULT_CS:
      digitalWrite(SPI_CSn_Default, HIGH);
      break;
    case MOD_CS:
      digitalWrite(SPI_CSn_Mod, HIGH);
      break;
    default:
      break;
  }
  SPI.endTransaction();
}

void DW3000_readData(uint8_t reg, uint8_t offset, uint8_t len, uint8_t *data, uint8_t cs_pin)
{
  uint8_t header[3] = {0, 0, 0};
  uint8_t header_len = 1;

  header[0] = READ;
  switch (offset)
  {
  case JUNK:
    // Short Addressed Transaction
    header[0] = ADDR_8 | (_reverseBit(reg) >> 1);
    break;
  default:
    // Full Addressed Transaction
    header[0] = ADDR_16 | (_reverseBit(reg) >> 1) | ((offset << 1) & ADDR_MASK1);
    offset = _reverseBit(offset);
    header[1] = (offset >> 2);
    header_len++;
    break;
  }
  // Data Read
  _readSPI(header_len, header, len, data, cs_pin);
}
void DW3000_writeData(uint8_t reg, uint8_t offset, uint8_t len, uint8_t *data, SPI_WRITE_TYPE wtype, uint8_t cs_pin)
{
  uint8_t header[3] = {0, 0, 0};
  uint8_t header_len = 1;

  switch (wtype)
  {
  case FAST:
    // Short Addressed Transaction
    header[0] = ADDR_8 | (_reverseBit(reg) >> 1) | FAST_CMD;
    break;
  case SHORT:
    // Short Addressed Transaction
    header[0] = ADDR_8 | (_reverseBit(reg) >> 1);
    break;
  case MASK_01:
  case MASK_10:
  case MASK_11:
  case FULL:
    // Full Addressed Transaction
    header[0] = ADDR_16 | (_reverseBit(reg) >> 1) | ((offset << 1) & ADDR_MASK1);
    offset = _reverseBit(offset);
    header[1] = (offset >> 2);
    header_len++;
    break;
  }
  header[0] |= WRITE;
  if (wtype == MASK_01)
  {
    header[1] = header[1] | M0;
  }
  if (wtype == MASK_10)
  {
    header[1] = header[1] | M1;
  }
  if (wtype == MASK_11)
  {
    header[1] = header[1] | M1 | M0;
  }
  // data reverse
  for (uint8_t i = 0; i < len; i++)
  {
    data[i] = _reverseBit(data[i]);
  }
  // Data Write
  _writeSPI(header_len, header, len, data, cs_pin);
}
void DW3000_setConfig(DW3000_SYS_CFG *sys_cfg, uint8_t cs_pin)
{
  SPI_WRITE_TYPE wtype = FULL;
  DW3000_writeData(GEN_CFG_AES_1, SYS_CFG, SYS_CFG_LEN, sys_cfg->data, wtype, cs_pin);
}
void DW3000_getConfig(uint8_t *sys_cfg, uint8_t cs_pin)
{

  DW3000_readData(GEN_CFG_AES_1, SYS_CFG, SYS_CFG_LEN, sys_cfg, cs_pin);
}
void DW3000_Spi_init()
{
  SPI.begin();
  // SET MISO
  pinMode(SPI_MISO, INPUT);
  // SET MOSI
  pinMode(SPI_MOSI, OUTPUT);
  // SET Slave Select
  pinMode(SPI_CSn_Default, OUTPUT);
  //pinMode(SPI_CSn_Mod, OUTPUT);
  digitalWrite(SPI_CSn_Default, HIGH);
  //digitalWrite(SPI_CSn_Mod, HIGH);
}

void DW3000_RST_init()
{
  pinMode(RSTn, OUTPUT);
  digitalWrite(RSTn, LOW);
  delay(10);
  digitalWrite(RSTn, HIGH);

  
}

void DW3000_init(DW3000_SYS_CFG *sys_cfg, uint8_t cs_pin)
{

  // SET SYS_CFG Register
  DW3000_setConfig(sys_cfg, cs_pin);
}
bool DW3000_getDevID(uint8_t cs_pin)
{
  uint8_t data[DEV_ID_LEN] = {
      0,
  };
  DW3000_readData(GEN_CFG_AES_1, DEV_ID, DEV_ID_LEN, data, cs_pin);
  uint8_t rev = data[0] & 0x0F;
  uint8_t ver = (data[0] & 0xF0) >> 4;
  uint8_t model = data[1];
  char ridtag[12];
  sprintf(ridtag, "%X", data[2] | (((uint16_t)data[3]) << 8));
  Serial.println("========== DW3000 DevID ==========");
  Serial.print("REV : ");
  Serial.println(rev);
  Serial.print("VER : ");
  Serial.println(ver);
  Serial.print("MODEL : ");
  Serial.println(model);
  Serial.print("RIDTAG : ");
  Serial.println(ridtag);
  if ((uint8_t(ridtag[0]) == 68u) && (uint8_t(ridtag[1]) == 69u) && (uint8_t(ridtag[2]) == 67u) && (uint8_t(ridtag[3]) == 65))
  {
    Serial.println("All right All right All right");
    return TRUE;
  }
  else
  {
    Serial.println("No not all right");
    return FALSE;
  }
}
void DW3000_getEUI(uint8_t cs_pin)
{
  uint8_t data[EUI_64_LEN] = {
      0,
  };
  DW3000_readData(GEN_CFG_AES_1, EUI_64, EUI_64_LEN, data, cs_pin);
  char eui[24];
  sprintf(eui, "%02X:%02X:%02X:%02X:%02X", data[4], data[3], data[2], data[1], data[0]);
  Serial.println("========== DW3000 EUI ==========");
  Serial.print("EUI : ");
  Serial.println(eui);
}
void DW3000_getOUI(uint8_t cs_pin)
{
  uint8_t data[EUI_64_LEN] = {
      0,
  };
  DW3000_readData(GEN_CFG_AES_1, EUI_64, EUI_64_LEN, data, cs_pin);
  char oui[12];
  sprintf(oui, "%02X:%02X:%02X", data[7], data[6], data[5]);
  Serial.println("========== DW3000 OUI ==========");
  Serial.print("OUI : ");
  Serial.println(oui);
}

// FAST Command
void DW3000_clearIDLE(uint8_t cs_pin)
{
  SPI_WRITE_TYPE wtype = FAST;
  DW3000_writeData(FAST_CMD_TXRXOFF, 0, 0, NULL, wtype, cs_pin);
}
void DW3000_startTX(uint8_t cs_pin)
{
  SPI_WRITE_TYPE wtype = FAST;
  DW3000_writeData(FAST_CMD_TX, 0, 0, NULL, wtype, cs_pin);
}
void DW3000_startRX(uint8_t cs_pin)
{
  SPI_WRITE_TYPE wtype = FAST;
  DW3000_writeData(FAST_CMD_RX, 0, 0, NULL, wtype, cs_pin);
}

void DW3000_writeCHAN_CTRL(uint8_t *txcctrl, uint8_t cs_pin)
{

  SPI_WRITE_TYPE wtype = FULL;
  DW3000_writeData(GEN_CFG_AES_2, CHAN_CTRL, CHAN_CTRL_LEN, txcctrl, wtype, cs_pin);
}

void DW3000_writeTXFCTRL(DW3000_TX_FCTRL *txfctrl, uint8_t cs_pin)
{
  uint8_t data[TX_FCTRL_LEN] = {0, };
  data[0] = txfctrl->tx_flen & 0xFF;
  data[1] = (txfctrl->tx_flen >> 8) | (txfctrl->tx_br << 2) | (txfctrl->tr << 3) | (txfctrl->tx_psr << 4);
  data[2] = txfctrl->txb_offset & 0xFF;
  data[3] = txfctrl->txb_offset >> 8;
  //data[4] = 0;
  data[5] = txfctrl->fine_plen;

  SPI_WRITE_TYPE wtype = FULL;
  DW3000_writeData(GEN_CFG_AES_1, TX_FCTRL, TX_FCTRL_LEN, data, wtype, cs_pin);
}

void DW3000_writeTXData(uint16_t len, uint8_t *data, uint16_t bufferOffset)
{
  // write short addrest because inside doesnt have offset it just a big ass buffer of 1024 bytes
  SPI_WRITE_TYPE wtype = SHORT;
  if (bufferOffset == 0x00)
  {
    DW3000_writeData(TX_BUFFER, 0, len, data, wtype, DEFAULT_CS);
  }
  else
  {
    wtype = FULL;
    DW3000_writeData(TX_BUFFER, bufferOffset, len, data, wtype, DEFAULT_CS);
  }
}
uint32_t DW3000_getSysStatus(uint8_t cs_pin)
{
  uint32_t sysStatus;
  uint8_t data[4] = {
      0,
  };
  DW3000_readData(GEN_CFG_AES_1, SYS_STATUS, 4, data, cs_pin);
  for (uint8_t i = 0; i < 4; i++)
  {
    sysStatus = sysStatus | (((uint32_t)data[i]) << (i * 8));
  }
  return sysStatus;
}
void DW3000_setSysStatus(uint32_t fullData, uint8_t cs_pin)
{
  uint8_t data[4] = {
      0,
  };
  for (uint8_t i = 0; i < 4; i++)
  {
    data[i] = (uint8_t)(fullData >> (i * 8));
  }
  SPI_WRITE_TYPE wtype = FULL;

  DW3000_writeData(GEN_CFG_AES_1, SYS_STATUS, 4, data, wtype, cs_pin);

}
uint16_t DW3000_getRXFrameLength(uint8_t cs_pin)
{
  uint8_t data[RX_FINFO_LEN] = {
      0,
  };
  DW3000_readData(GEN_CFG_AES_1, RX_FINFO, RX_FINFO_LEN, data, cs_pin);

  uint16_t frame_len = (data[1] & 0x03) | data[0];
  return frame_len;
}
void DW3000_getRXData(uint16_t len, uint8_t *data, uint16_t bufferOffset, uint8_t cs_pin)
{
  DW3000_readData(RX_BUFFER_0, bufferOffset, len, data, cs_pin);
}
