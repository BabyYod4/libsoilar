
#include "rfm95w.h"

#if LIB_TYPE == LIB_C

/// Private Variables
static Rfm95wInterface* _interfaceRfm95[LES_MAX_INSTANCES];
static uint8_t _indexRfm95w[LES_MAX_INSTANCES] = {0};

/// Lora Device Interface Logic 

LoraEndnodeCodes readPacketRfm95w(uint8_t* output, uint8_t size ){
  if ( _loraEndnodeSettings[LES_CIU]->lowPowerReceiveMode ){
    _waitTillCallbackReadyRfm95w();
  }

  uint8_t i = 0;
  while( ( (_readRegisterRfm95w( RFM95W_REG_RX_NB_BYTES) - _indexRfm95w[LES_CIU]) > 0 ) && (i < size) ){
      output[i] = _readRfm95w();
      i++;
  }

  if (output[0] != 0xFF){ return LEC_PKG_CORRUPTED; }

  return LEC_PKG_RECEIVED;
}

LoraEndnodeCodes sendPacketRfm95w(uint8_t* input, uint8_t size){
  // Start Transmission
  if ( !_startTransmissionRfm95w() ){ return LEC_ALREADY_TRANSMITTING; }
  
  // Get payload size settings
  uint8_t currentLength = _readRegisterRfm95w( RFM95W_REG_PAYLOAD_LENGTH);

  // Check packet size and crop if neccesary
  if ( (currentLength + size) > RFM95W_MAX_PKT_LENGTH ){
      size = RFM95W_MAX_PKT_LENGTH - currentLength;
  }

  // Write data
  for( uint8_t i = 0; i < size; i++){
      _writeRegisterRfm95w( RFM95W_REG_FIFO, input[i]);
  }

  // Update length
  _writeRegisterRfm95w(  RFM95W_REG_PAYLOAD_LENGTH, currentLength + size);

  // Stop transmission
  _stopTransmissionRfm95w(  );
  return LEC_DONE_TRANSMITTING;

}

LoraEndnodeModes getModeRfm95w(){
  return _loraEndnodeModes[LES_CIU];
}

void setModeRfm95w(LoraEndnodeModes newMode){
  switch( newMode ){
    case LEM_INIT_MODE: // This has no real benefit or effect. 
      _loraEndnodeModes[LES_CIU] = LEM_INIT_MODE;
      break;
    case LEM_STDBY_MODE:
      _writeRegisterRfm95w( RFM95W_REG_OP_MODE, RFM95W_MODE_LONG_RANGE_MODE | RFM95W_MODE_STDBY);
      _loraEndnodeModes[LES_CIU] = LEM_STDBY_MODE;
      removeInterrupt (_interfaceRfm95[LES_CIU]->dio0 );
      break;
    case LEM_SLEEP_MODE:
      _writeRegisterRfm95w( RFM95W_REG_OP_MODE, RFM95W_MODE_LONG_RANGE_MODE | RFM95W_MODE_SLEEP);
      _loraEndnodeModes[LES_CIU] = LEM_SLEEP_MODE;
      removeInterrupt (_interfaceRfm95[LES_CIU]->dio0 );
      break;
    case LEM_TX_MODE:
      _writeRegisterRfm95w( RFM95W_REG_OP_MODE, RFM95W_MODE_LONG_RANGE_MODE | RFM95W_MODE_TX);
      _loraEndnodeModes[LES_CIU] = LEM_TX_MODE;
      removeInterrupt (_interfaceRfm95[LES_CIU]->dio0 );
      break;
    case LEM_RX_MODE:
      _writeRegisterRfm95w( RFM95W_REG_OP_MODE, RFM95W_MODE_LONG_RANGE_MODE | RFM95W_MODE_RX_SINGLE);
      _loraEndnodeModes[LES_CIU] = LEM_RX_MODE;
      removeInterrupt (_interfaceRfm95[LES_CIU]->dio0 );
      break;
    case LEM_RX_LOW_POWER_MODE:
      if ( !_loraEndnodeSettings[LES_CIU]->lowPowerReceiveMode ){ throwException("ERROR: lowPowerReceiveMode not set to True, unable to attach interrupt!"); }
      _writeRegisterRfm95w( RFM95W_REG_DIO_MAPPING_1, 0x00); // DIO mapping 0
      addInterrupt( _interfaceRfm95[LES_CIU]->dio0, _loraEndnodeOnReceiveCallbacks[LES_CIU], ON_RISE );

      // check if lowPowerReceivedMode is enabled
      if ( _loraEndnodeSettings[LES_CIU]->lowPowerReceiveMode ){
        _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1, _readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1) & 0xfe);
      
        // set RX Continuous mode a.k.a. RX Low Power Mode 
        _writeRegisterRfm95w( RFM95W_REG_OP_MODE, RFM95W_MODE_LONG_RANGE_MODE | RFM95W_MODE_RX_CONTINUOUS);
        _loraEndnodeModes[LES_CIU] = LEM_RX_LOW_POWER_MODE;
      }
      break;
  }
}

LoraEndnodeMetaData getMetaDataRfm95w(){
  int16_t rssi = (_readRegisterRfm95w( RFM95W_REG_PKT_RSSI_VALUE) - 134 );
  int8_t snr = ((int8_t)_readRegisterRfm95w( RFM95W_REG_PKT_SNR_VALUE)) / 4;

#if MODE_TYPE == NORMAL
    int32_t freqErrorHz = 0;
    int32_t freqErrorBytes = 0;

    freqErrorBytes = _readRegisterRfm95w( RFM95W_REG_FREQ_ERROR_MSB);
    freqErrorBytes <<= 8;
    freqErrorBytes |= _readRegisterRfm95w( RFM95W_REG_FREQ_ERROR_MID);
    freqErrorBytes <<= 8;
    freqErrorBytes |= _readRegisterRfm95w( RFM95W_REG_FREQ_ERROR_LSB);
    
    if (freqErrorBytes & 0x80000) { freqErrorBytes |= 0xfff00000; }

    freqErrorHz = 0; // In hertz
    uint8_t bwindex = _readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1) >> 4;
    if (bwindex < (sizeof(RFM95W_BW_TAB) / sizeof(float)))
	  freqErrorHz = (float)freqErrorBytes * bw_tab[bwindex] * ((float)(1L << 24) / (float)RFM95_FXOSC / 500.0);
    return LoraEndnodeMetaData{ rssi, snr, freqErrorHz };
#endif //MODE_TYPE_NORMAL

  return LoraEndnodeMetaData{ rssi, snr };
}

void hopRfm95w( LoraEndnodeSettings* newSettings ){
  // setModeRfm95w(  LORA_STDBY_MODE );
  _loraEndnodeSettings[LES_CIU] = newSettings;
  _initRfm95w();
}

bool packetReceivedRfm95w(){
  uint8_t tmp = _parsePacketRfm95w();
  if (tmp > 0){
    return true;
  } else{ return false; }
}

void addOnReceiveCallbackRfm95w( void (*callback)(void) ){
  if ( !_loraEndnodeSettings[LES_CIU]->lowPowerReceiveMode ){ throwException("ERROR: lowPowerReceiveMode not set to True, unable to attach interrupt!"); }
  _loraEndnodeOnReceiveCallbacks[LES_CIU] = callback;
}


LoraEndnodeCodes CreateRfm95W(LoraEndnode* self, Rfm95wInterface* interface, LoraEndnodeSettings* settings, uint8_t instanceNum ){
  if( instanceNum >= LES_MAX_INSTANCES ){ throwException("ERROR: unable to init RFM95W LoraEndnodeDevice becaue to many LoraEndnodeDevice made"); }
  
  _loraEndnodeSettings[instanceNum] = settings;
  _interfaceRfm95[instanceNum] = interface;

  self->read = &readPacketRfm95w;
  self->send = &sendPacketRfm95w;
  self->getMode = &getModeRfm95w;
  self->setMode = &setModeRfm95w;
  self->getMetaData = &getMetaDataRfm95w;
  self->hop = &hopRfm95w;
  self->packetReceived = &packetReceivedRfm95w;
  self->addOnReceiveCallback = &addOnReceiveCallbackRfm95w;
  self->instance = instanceNum;

  bool error = _initRfm95w();
  if (error) { return LEC_ERROR_WRONG_DEVICE; }
  return LEC_INIT_DONE;
}


/// Module Specific Logic
bool _initRfm95w(){
  // setup pins
  gpioInit( _interfaceRfm95[LES_CIU]->nss, GPIO_OUT );
  gpioInit(_interfaceRfm95[LES_CIU]->reset, GPIO_OUT );

  // set NSS high
  gpioSet(_interfaceRfm95[LES_CIU]->nss, HIGH);

  // Manual reset (see manual 7.2)
  if (_interfaceRfm95[LES_CIU]->reset != -1 ) {
    // Wait for POR (See manual 7.2.1)
    gpioSet(_interfaceRfm95[LES_CIU]->reset, LOW);
    sleepMs(10);
    gpioSet(_interfaceRfm95[LES_CIU]->reset, HIGH);
    gpioInit(_interfaceRfm95[LES_CIU]->reset, GPIO_IN );

    // start spi interface
    spiInit( _interfaceRfm95[LES_CIU]->spiDev );
  }


  // check version
  uint8_t version = _readRegisterRfm95w( RFM95W_REG_VERSION);
  if (version != 0x12) {
    return true;
  }

  // put in sleep mode
  setModeRfm95w(  LEM_SLEEP_MODE );

  // set base addresses
  _writeRegisterRfm95w( RFM95W_REG_FIFO_TX_BASE_ADDR, 0);
  _writeRegisterRfm95w( RFM95W_REG_FIFO_RX_BASE_ADDR, 0);

  // set LNA boost
  _writeRegisterRfm95w( RFM95W_REG_LNA, _readRegisterRfm95w( RFM95W_REG_LNA ) | 0x03);

  // set auto AGC
  _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_3, 0x04);

  // put in standby mode
  setModeRfm95w(  LEM_STDBY_MODE );

  // set settings
  _setSettings();

  return false;
}


uint8_t _parsePacketRfm95w(){
  uint8_t packetLength = 0;
  uint8_t irqFlags = _readRegisterRfm95w( RFM95W_REG_IRQ_FLAGS);

  _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1, _readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1) & 0xfe);

  // clear IRQ's
  _writeRegisterRfm95w( RFM95W_REG_IRQ_FLAGS, irqFlags);

  if ((irqFlags & RFM95W_IRQ_RX_DONE_MASK) && (irqFlags & RFM95W_IRQ_PAYLOAD_CRC_ERROR_MASK) == 0) {
    // received a packet
    _indexRfm95w[LES_CIU] = 0;

    packetLength = _readRegisterRfm95w( RFM95W_REG_RX_NB_BYTES);
  
    // set FIFO address to current RX address
    _writeRegisterRfm95w( RFM95W_REG_FIFO_ADDR_PTR, _readRegisterRfm95w( RFM95W_REG_FIFO_RX_CURRENT_ADDR));

    // put in standby mode
    setModeRfm95w( LEM_STDBY_MODE );
  } else if (_readRegisterRfm95w( RFM95W_REG_OP_MODE) != (RFM95W_MODE_LONG_RANGE_MODE | RFM95W_MODE_RX_SINGLE)) {
    // not currently in RX mode

    // reset FIFO address
    _writeRegisterRfm95w( RFM95W_REG_FIFO_ADDR_PTR, 0);

    // put in single RX mode
    setModeRfm95w( LEM_RX_MODE );
  }

  return packetLength;

}

uint8_t _readRfm95w(){
  if ( !( ( _readRegisterRfm95w( RFM95W_REG_RX_NB_BYTES) - _indexRfm95w[LES_CIU]) > 0 ) ) {
    return (uint8_t)-1;
  }

  _indexRfm95w[LES_CIU]++;

  return _readRegisterRfm95w( RFM95W_REG_FIFO);
}

void _setOCPRfm95w(uint8_t mA){
  uint8_t ocpTrim = 27;
  if (mA <= 120) { ocpTrim = (mA - 45) / 5; } 
  else if (mA <=240) { ocpTrim = (mA + 30) / 10; }

  _writeRegisterRfm95w( RFM95W_REG_OCP, 0x20 | (0x1F & ocpTrim));
}

bool _isTransmittingRfm95w(){
  if ((_readRegisterRfm95w( RFM95W_REG_OP_MODE) & RFM95W_MODE_TX) == RFM95W_MODE_TX) {
    return true;
  }

  if (_readRegisterRfm95w( RFM95W_REG_IRQ_FLAGS) & RFM95W_IRQ_TX_DONE_MASK) {
    // clear IRQ's
    _writeRegisterRfm95w( RFM95W_REG_IRQ_FLAGS, RFM95W_IRQ_TX_DONE_MASK);
  }

  return false;
}

uint8_t _readRegisterRfm95w(uint8_t address){
  return _singleTransferRfm95w( address & 0x7f, 0x00);
}

void _writeRegisterRfm95w(uint8_t address, uint8_t value){
  _singleTransferRfm95w( address | 0x80, value);
}

uint8_t _singleTransferRfm95w(uint8_t address, uint8_t value){
  uint8_t response;

  gpioSet(_interfaceRfm95[LES_CIU]->nss, LOW);

  spiAquire( _interfaceRfm95[LES_CIU]->spiDev, SPI_CLK_8MHZ, SPI_MODE_0, SPI_ORDER_MSB );
  spiTransfer(_interfaceRfm95[LES_CIU]->spiDev, address);
  response = spiTransfer(_interfaceRfm95[LES_CIU]->spiDev, value);
  spiRelease( _interfaceRfm95[LES_CIU]->spiDev );

  gpioSet(_interfaceRfm95[LES_CIU]->nss, HIGH);

  return response;
}

void _setSettings(){
/// Setting carrier frequency
  byte3_t frequency = _loraEndnodeSettings[LES_CIU]->frequencyMHz;
  _writeRegisterRfm95w( RFM95W_REG_FRF_MSB, frequency.msb);
  _writeRegisterRfm95w( RFM95W_REG_FRF_MID, frequency.mid);
  _writeRegisterRfm95w( RFM95W_REG_FRF_LSB, frequency.lsb);

/// Setting TxPower
  uint8_t level = _loraEndnodeSettings[LES_CIU]->txPowerdBm;
  if (level > 17) { 
    if (level > 20) { level = 20; }

    // subtract 3 from level, so 18 - 20 maps to 15 - 17
    level -= 3;

    // High Power +20 dBm Operation (Read manual 5.4.3.)
    _writeRegisterRfm95w( RFM95W_REG_PA_DAC, 0x87);
    _setOCPRfm95w( 140);
  } else {
    if (level < 2) {
      level = 2;
    }
    // Default value PA_HF/LF or +17dBm
    _writeRegisterRfm95w( RFM95W_REG_PA_DAC, 0x84);
    _setOCPRfm95w( 100);
  }

  _writeRegisterRfm95w( RFM95W_REG_PA_CONFIG, RFM95W_PA_BOOST | (level - 2));

/// Setting Bandwidth
  _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1, (_readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1) & 0x0f) | (_loraEndnodeSettings[LES_CIU]->signalBandwidthHz << 4));

/// Setting Spreading Factor (SF)
  uint8_t sf = _loraEndnodeSettings[LES_CIU]->spreadingFactor;
  if (sf < 6) { sf = 6; } // Limit SF to range 6-12
  else if (sf > 12) { sf = 12; }

  if (sf == 6) {
    _writeRegisterRfm95w( RFM95W_REG_DETECTION_OPTIMIZE, 0xc5);
    _writeRegisterRfm95w( RFM95W_REG_DETECTION_THRESHOLD, 0x0c);
  } else {
    _writeRegisterRfm95w( RFM95W_REG_DETECTION_OPTIMIZE, 0xc3);
    _writeRegisterRfm95w( RFM95W_REG_DETECTION_THRESHOLD, 0x0a);
  }

  _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_2, (_readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));

/// Setting Preamble Length
  byte2_t length = _loraEndnodeSettings[LES_CIU]->preambleLength;
  _writeRegisterRfm95w( RFM95W_REG_PREAMBLE_MSB, length.msb);
  _writeRegisterRfm95w( RFM95W_REG_PREAMBLE_LSB, length.lsb);

/// Setting Code Rate
  uint8_t cr = _loraEndnodeSettings[LES_CIU]->codeRate;
  if (cr < 5) { cr = 5; } /* Limit CR to {4/5 ... 4/8} */
  else if (cr > 8) { cr = 8; }
  _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1, (_readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));

/// Setting Sync Word
  _writeRegisterRfm95w( RFM95W_REG_SYNC_WORD, _loraEndnodeSettings[LES_CIU]->syncWord);

/// Enable/Disable CRC
  if ( _loraEndnodeSettings[LES_CIU]->enableCRC ){
    _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_2, _readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_2) | 0x04);
  } else{
    _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_2, _readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_2) & 0xfb);
  }

/// Enable/Disable InvertIQ
  if ( _loraEndnodeSettings[LES_CIU]->invertIQ ){
    _writeRegisterRfm95w( RFM95W_REG_INVERTIQ,  0x66);
    _writeRegisterRfm95w( RFM95W_REG_INVERTIQ2, 0x19);
  } else{
    _writeRegisterRfm95w( RFM95W_REG_INVERTIQ,  0x27);
    _writeRegisterRfm95w( RFM95W_REG_INVERTIQ2, 0x1d);
  }  
}

void _waitTillCallbackReadyRfm95w(  ){
  uint8_t irqFlags = _readRegisterRfm95w( RFM95W_REG_IRQ_FLAGS);

  // clear IRQ's
  _writeRegisterRfm95w( RFM95W_REG_IRQ_FLAGS, irqFlags);
  if ((irqFlags & RFM95W_IRQ_PAYLOAD_CRC_ERROR_MASK) == 0){
    _indexRfm95w[LES_CIU] = 0;

    // set FIFO address to current RX address
    _writeRegisterRfm95w( RFM95W_REG_FIFO_ADDR_PTR, _readRegisterRfm95w( RFM95W_REG_FIFO_RX_CURRENT_ADDR));
  }

}

bool _startTransmissionRfm95w(){
  if (_isTransmittingRfm95w()) {
    return false;
  }

  // put in standby mode
  setModeRfm95w(  LEM_STDBY_MODE );

  _writeRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1, _readRegisterRfm95w( RFM95W_REG_MODEM_CONFIG_1) & 0xfe);

  // reset FIFO address and paload length
  _writeRegisterRfm95w( RFM95W_REG_FIFO_ADDR_PTR, 0);
  _writeRegisterRfm95w( RFM95W_REG_PAYLOAD_LENGTH, 0);

  return true;
}

void _stopTransmissionRfm95w(){
  // put in TX mode
  setModeRfm95w(  LEM_TX_MODE );

  // wait for TX done
  while ((_readRegisterRfm95w( RFM95W_REG_IRQ_FLAGS) & RFM95W_IRQ_TX_DONE_MASK) == 0) {
    yieldThread();
  }
  // clear IRQ's
  _writeRegisterRfm95w( RFM95W_REG_IRQ_FLAGS, RFM95W_IRQ_TX_DONE_MASK);
}

#endif //LIB_TYPE_C
