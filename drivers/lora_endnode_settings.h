#ifndef LORA_ENDNODE_SETTINGS_H
#define LORA_ENDNODE_SETTINGS_H

#include "../hal/hal.h"
#include "../tools/data_types/byte_types.h"

#define LES_MAX_INSTANCES   (uint8_t)2
static uint8_t LES_CIU = 0;

typedef enum LoraEndnodeCodes {  
    LEC_ALREADY_TRANSMITTING, 
    LEC_DONE_TRANSMITTING, 
    LEC_INIT_DONE,
    LEC_ERROR_GENERIC, 
    LEC_ERROR_UNABLE_TO_INIT,
    LEC_ERROR_TO_MANY_DEVICES,
    LEC_ERROR_WRONG_DEVICE,
    LEC_PKG_CORRUPTED,
    LEC_PKG_RECEIVED
} LoraEndnodeCodes;

typedef enum LoraEndnodeModes {
    LEM_INIT_MODE,
    LEM_STDBY_MODE,
    LEM_SLEEP_MODE,
    LEM_TX_MODE,
    LEM_RX_MODE,
    LEM_RX_LOW_POWER_MODE
} LoraEndnodeModes;

typedef struct LoraEndnodeMetaData LoraEndnodeMetaData;
struct LoraEndnodeMetaData{
    int16_t rssi; 
    int8_t snr; 
#if MODE_TYPE == NORMAL
    int32_t freqErrorHz;
#endif //MODE_TYPE_NORMAL
};

typedef struct LoraEndnodeSettings LoraEndnodeSettings;
struct LoraEndnodeSettings { 
    byte3_t frequencyMHz;
    uint8_t signalBandwidthHz;
    byte2_t preambleLength;
    uint8_t txPowerdBm;
    uint8_t spreadingFactor;
    uint8_t codeRate;
    uint8_t syncWord;
    bool enableCRC;
    bool lowPowerReceiveMode; // enable for 'pkt received callback' on dio0 rise.
    bool invertIQ;
};


static void SelectLoraEndnodeDevice(uint8_t dev){ LES_CIU = dev; }

typedef void (*LoraEndnodeCallback)(void); 
static LoraEndnodeSettings* _loraEndnodeSettings[LES_MAX_INSTANCES];
static LoraEndnodeCallback _loraEndnodeOnReceiveCallbacks[LES_MAX_INSTANCES];
static LoraEndnodeModes _loraEndnodeModes[LES_MAX_INSTANCES] = { LEM_INIT_MODE };



#endif //LORA_ENDNODE_SETTINGS_H