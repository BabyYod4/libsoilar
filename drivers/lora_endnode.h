#ifndef LORA_ENDNODE_H
#define LORA_ENDNODE_H

#include "lora_endnode_settings.h"

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

    uint8_t instance;
};

#endif //LORA_ENDNODE_H