#ifndef RFM95W_SETTINGS_H
#define RFM95W_SETTINGS_H

#include "../../tools/data_types/byte_types.h"

//Max size Lora Packet 
#define RFM95W_MAX_PKT_LENGTH                     255 
// Max instances of the Rfm95W objectsthat can be made
#define RFM95_FXOSC                               32000000.0


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

// 865.20 MHZ
#define RFM95W_CHANNEL_868_10                     byte3_t{0xD8, 0x4C, 0xCD}
// 865.50 MHZ
#define RFM95W_CHANNEL_868_11                     byte3_t{0xD8, 0x60, 0x00}
// 865.80 MHZ
#define RFM95W_CHANNEL_868_12                     byte3_t{0xD8, 0x73, 0x33}
// 866.10 MHZ
#define RFM95W_CHANNEL_868_13                     byte3_t{0xD8, 0x86, 0x66}
// 866.40 MHZ
#define RFM95W_CHANNEL_868_14                     byte3_t{0xD8, 0x99, 0x9A}
// 866.70 MHZ
#define RFM95W_CHANNEL_868_15                     byte3_t{0xD8, 0xAC, 0xCD}
// 867 MHZ
#define RFM95W_CHANNEL_868_16                     byte3_t{0xD8, 0xC0, 0x00}
// 868 MHZ
#define RFM95W_CHANNEL_868_17                     RFM95W_FREQ_868MHZ

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

#endif //RFM95W_SETTINGS_H