#ifndef TYPE_CONVERSION_H
#define TYPE_CONVERSION_H

#include "../../hal/hal.h"

bool isLittleEndian(void);

void byteToString( uint8_t input, char output[2]);

uint8_t stringToByte( char input[2] );

void byte16ToString ( uint16_t input, char output[3]);

uint16_t stringToByte16( char input[3] );

void byte32ToString ( uint32_t input, char output[5]);

uint32_t stringToByte32( char input[5] );

void byte64ToString ( uint64_t input, char output[9]);

uint64_t stringToByte64( char input[9] );


// Bytestream to Bytes

void byteToByteStream( uint8_t input, uint8_t output[1]);

uint8_t byteStreamToByte( uint8_t input[1] );

void byte16ToByteStream ( uint16_t input, uint8_t output[2]);

uint16_t byteStreamToByte16( uint8_t input[2] );

void byte32ToBytestream ( uint32_t input, uint8_t output[4]);

uint32_t byteStreamToByte32( uint8_t input[4] );

void byte64ToByteStream ( uint64_t input, uint8_t output[8]);

uint64_t byteStreamToByte64( uint8_t input[8] );


#endif //TYPE_CONVERSION_H
