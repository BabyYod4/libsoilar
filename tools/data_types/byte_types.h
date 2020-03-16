#ifndef BYTE_TYPES_H
#define BYTE_TYPES_H

#include "../../hal/hal.h"

typedef struct byte2_t { uint8_t msb; uint8_t lsb; } byte2_t;
typedef struct byte3_t { uint8_t msb; uint8_t mid; uint8_t lsb; } byte3_t;

// bool cmpByte2( byte2_t lhs, byte2_t rhs ) {  return (lhs.msb == rhs.msb) && (lhs.lsb == rhs.lsb); }
// bool cmpByte3( byte3_t lhs, byte3_t rhs ) { return (lhs.msb == rhs.msb) && (lhs.mid == rhs.mid) && (lhs.lsb == rhs.lsb); }

#endif //BYTE_TYPES_H