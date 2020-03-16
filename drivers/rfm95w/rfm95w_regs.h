#ifndef REGS_H
#define REGS_H

// registers
#define RFM95W_REG_FIFO                 0x00
#define RFM95W_REG_OP_MODE              0x01
#define RFM95W_REG_FRF_MSB              0x06
#define RFM95W_REG_FRF_MID              0x07
#define RFM95W_REG_FRF_LSB              0x08
#define RFM95W_REG_PA_CONFIG            0x09
#define RFM95W_REG_OCP                  0x0b
#define RFM95W_REG_LNA                  0x0c
#define RFM95W_REG_FIFO_ADDR_PTR        0x0d
#define RFM95W_REG_FIFO_TX_BASE_ADDR    0x0e
#define RFM95W_REG_FIFO_RX_BASE_ADDR    0x0f
#define RFM95W_REG_FIFO_RX_CURRENT_ADDR 0x10
#define RFM95W_REG_IRQ_FLAGS            0x12
#define RFM95W_REG_RX_NB_BYTES          0x13
#define RFM95W_REG_PKT_SNR_VALUE        0x19
#define RFM95W_REG_PKT_RSSI_VALUE       0x1a
#define RFM95W_REG_MODEM_CONFIG_1       0x1d
#define RFM95W_REG_MODEM_CONFIG_2       0x1e
#define RFM95W_REG_PREAMBLE_MSB         0x20
#define RFM95W_REG_PREAMBLE_LSB         0x21
#define RFM95W_REG_PAYLOAD_LENGTH       0x22
#define RFM95W_REG_MODEM_CONFIG_3       0x26
#define RFM95W_REG_FREQ_ERROR_MSB       0x28
#define RFM95W_REG_FREQ_ERROR_MID       0x29
#define RFM95W_REG_FREQ_ERROR_LSB       0x2a
#define RFM95W_REG_RSSI_WIDEBAND        0x2c
#define RFM95W_REG_DETECTION_OPTIMIZE   0x31
#define RFM95W_REG_INVERTIQ             0x33
#define RFM95W_REG_DETECTION_THRESHOLD  0x37
#define RFM95W_REG_SYNC_WORD            0x39
#define RFM95W_REG_INVERTIQ2            0x3b
#define RFM95W_REG_DIO_MAPPING_1        0x40
#define RFM95W_REG_VERSION              0x42
#define RFM95W_REG_PA_DAC               0x4d

// modes
#define RFM95W_MODE_LONG_RANGE_MODE     0x80
#define RFM95W_MODE_SLEEP               0x00
#define RFM95W_MODE_STDBY               0x01
#define RFM95W_MODE_TX                  0x03
#define RFM95W_MODE_RX_CONTINUOUS       0x05
#define RFM95W_MODE_RX_SINGLE           0x06

// PA config
#define RFM95W_PA_BOOST                     0x80
#define RFM95W_PA_OUTPUT_RFO_PIN            true
#define RFM95W_PA_OUTPUT_PA_BOOST_PIN       false 

// IRQ masks
#define RFM95W_IRQ_TX_DONE_MASK             0x08
#define RFM95W_IRQ_PAYLOAD_CRC_ERROR_MASK   0x20
#define RFM95W_IRQ_RX_DONE_MASK             0x40

#endif //REGS_H