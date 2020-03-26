// Copyright (c) Lucidy 2018

#ifndef LORA_H
#define LORA_H

#include "../lora_endnode.h"
#include "rfm95w_regs.h"
#include "rfm95w_settings.h"

typedef struct Rfm95wInterface Rfm95wInterface;
struct Rfm95wInterface { 
    spi_t spiDev;
    gpio_t nss; // Chip select a.k.a Slave Select
    gpio_t reset; 
    gpio_t dio0;
};

/// LoraEndnode Interface Logic 
LoraEndnodeCodes readPacketRfm95w(uint8_t* output, uint8_t size);
LoraEndnodeCodes sendPacketRfm95w(uint8_t* input, uint8_t size);

LoraEndnodeModes getModeRfm95w();
void setModeRfm95w(LoraEndnodeModes newMode);

LoraEndnodeMetaData getMetaDataRfm95w();
void hopRfm95w( LoraEndnodeSettings* newSettings);

bool packetReceivedRfm95w();
void addOnReceiveCallbackRfm95w( void (*callback)(void) );

LoraEndnodeCodes CreateRfm95W(LoraEndnode* self, Rfm95wInterface* interface, LoraEndnodeSettings* settings, uint8_t instanceNum);


/// Module Specific Logic
inline bool _initRfm95w();
inline uint8_t _parsePacketRfm95w();
inline uint8_t _readRfm95w();

inline void _setOCPRfm95w(uint8_t mA);
inline bool _isTransmittingRfm95w();
inline void _setSettings();
inline void _waitTillCallbackReadyRfm95w();

inline uint8_t _readRegisterRfm95w(uint8_t address);
inline void _writeRegisterRfm95w(uint8_t address, uint8_t value);
inline uint8_t _singleTransferRfm95w(uint8_t address, uint8_t value);

inline bool _startTransmissionRfm95w();
inline void _stopTransmissionRfm95w();

#endif //LORA_H
