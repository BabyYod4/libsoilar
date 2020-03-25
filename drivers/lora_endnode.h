#ifndef LORA_ENDNODE_H
#define LORA_ENDNODE_H

#include "../hal/hal.h"
#include "../tools/data_types/byte_types.h"

typedef enum LoraEndnodeCodes {  
    LORA_DEV_ALREADY_TRANSMITTING, 
    LORA_DEV_DONE_TRANSMITTING, 
    LORA_INIT_DONE,
    LORA_ERROR_GENERIC, 
    LORA_ERROR_UNABLE_TO_INIT,
    LORA_ERROR_WRONG_DEVICE,
    LORA_PKG_CORRUPTED,
    LORA_PKG_RECEIVED
} LoraEndnodeCodes;

typedef enum LoraEndnodeModes {
    LORA_INIT_MODE,
    LORA_STDBY_MODE,
    LORA_SLEEP_MODE,
    LORA_TX_MODE,
    LORA_RX_MODE,
    LORA_RX_LOW_POWER_MODE
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
    bool invertIQ; // 
};

typedef struct LoraEndnode LoraEndnode;
struct LoraEndnode {
    LoraEndnodeCodes (*read)(uint8_t*, uint8_t);
    LoraEndnodeCodes (*send)(uint8_t*, uint8_t);
    
    LoraEndnodeModes (*getMode)();
    void (*setMode)(LoraEndnodeModes);

    LoraEndnodeMetaData (*getMetaData)();
    void (*hop)(LoraEndnodeSettings*); 
    
    bool (*packetReceived)();
    void (*addOnReceiveCallback)( void (*callback)(void) );
};

#endif //LORA_ENDNODE_H