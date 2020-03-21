#ifndef RFM95W_SETTINGS_H
#define RFM95W_SETTINGS_H

#include "../../tools/data_types/byte_types.h"

//Max size Lora Packet 
#define RFM95W_MAX_PKT_LENGTH                      255 
#define RFM95W_FXOSC                               32000000.0
#define RFM95W_FSTEP                               (RFM95_FXOSC / 524288)

#define RFM95W_CODE_RATE_4_5                      0x01
#define RFM95W_CODE_RATE_4_6                      0x02
#define RFM95W_CODE_RATE_4_7                      0x03
#define RFM95W_CODE_RATE_4_8                      0x04

#define RFM95W_BW_7_8KHZ                          0x00
#define RFM95W_BW_10_4KHZ                         0x01
#define RFM95W_BW_15_6KHZ                         0x02
#define RFM95W_BW_20_8KHZ                         0x03 
#define RFM95W_BW_31_25KHZ                        0x04
#define RFM95W_BW_41_7KHZ                         0x05
#define RFM95W_BW_62_5KHZ                         0x06
#define RFM95W_BW_125KHZ                          0x07
#define RFM95W_BW_250KHZ                          0x08
#define RFM95W_BW_500KHZ                          0x09

#define RFM95W_FREQ_433MHZ                        byte3_t{0x6C, 0x80, 0x00}
#define RFM95W_FREQ_868MHZ                        byte3_t{0xD9, 0x00, 0x00}
#define RFM95W_FREQ_915MHZ                        byte3_t{0x16, 0xE0, 0x00}

// 875.8125 MHZ
#define RFM95W_CHANNEL_868_1                     byte3_t{0xDA, 0xF4, 0x00}
// 875.9375 MHZ
#define RFM95W_CHANNEL_868_2                     byte3_t{0xDA, 0xFC, 0x00}
// 876.0625 MHZ
#define RFM95W_CHANNEL_868_3                     byte3_t{0xDB, 0x04, 0x00}
// 876.1875 MHZ
#define RFM95W_CHANNEL_868_4                     byte3_t{0xDB, 0x0C, 0x00}

// 866.8125 MHZ
#define RFM95W_CHANNEL_868_5                     byte3_t{0xD8, 0xB4, 0x00}
// 866.9375 MHZ
#define RFM95W_CHANNEL_868_6                     byte3_t{0xD8, 0xBC, 0x00}
// 867.0625 MHZ
#define RFM95W_CHANNEL_868_7                     byte3_t{0xD8, 0xC4, 0x00}
// 867.1875 MHZ
#define RFM95W_CHANNEL_868_8                     byte3_t{0xD8, 0xCC, 0x00}

#define RFM95W_CHANNEL_868_TX                    byte3_t{0xD8, 0xB8, 0x00}

#define RFM95W_DYNAMIC_PKT_LENGTH                 0

#define RFM95W_DEFAULT_FREQ                       RFM95W_FREQ_868MHZ
#define RFM95W_DEFAULT_SIGNAL_BW                  RFM95W_BW_125KHZ
#define RFM95W_DEFAULT_PREAMBLE_LENGTH            byte2_t{0x00, 0x08}  
#define RFM95W_DEFAULT_TX_POWER                   17
#define RFM95W_DEFAULT_SF                         7
#define RFM95W_DEFAULT_CODE_RATE                  RFM95W_CODE_RATE_4_5
#define RFM95W_DEFAULT_SYNC_WORD                  0x12
#define RFM95W_DEFAULT_CRC_MODE                   false
#define RFM95W_DEFAULT_LOW_POWER_RECIEV_MODE      false
#define RFM95W_DEFAULT_INVERT_IQ                  false
#define RFM95W_DEFAULT_PKT_LENGTH                 RFM95W_DYNAMIC_PKT_LENGTH
#define RFM95W_DEFAULT_SETTINGS                   RFM95W_DEFAULT_FREQ, RFM95W_DEFAULT_SIGNAL_BW, RFM95W_DEFAULT_PREAMBLE_LENGTH, RFM95W_DEFAULT_TX_POWER, RFM95W_DEFAULT_SF, RFM95W_DEFAULT_CODE_RATE, RFM95W_DEFAULT_PKT_LENGTH, RFM95W_DEFAULT_SYNC_WORD, RFM95W_DEFAULT_CRC_MODE, RFM95W_DEFAULT_LOW_POWER_RECIEV_MODE, RFM95W_DEFAULT_INVERT_IQ  

const float RFM95W_BW_TAB[] = {7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250, 500};

static byte3_t RFM95W_CALC_FEQ_FAST( long freq ){
    byte3_t tmp;
    uint64_t frf = ((uint64_t)freq << 19) / 32000000;

    tmp.msb = (uint8_t)(frf >> 16);
    tmp.mid = (uint8_t)(frf >> 8);
    tmp.lsb = (uint8_t)(frf >> 0);
    // printf( "%04x %04x %04x", tmp.msb, tmp.mid, tmp.lsb );
    return tmp;
};

#endif //RFM95W_SETTINGS_H