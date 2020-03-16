#include "type_conversion.h"

bool isLittleEndian(void){
    int n = 1;
    if(*(char *)&n == 1){return true;}
    return false;
}


/// String 
void byteToString( uint8_t input, char output[2]){
    output[0] = (char)input;
    output[1] = '\0';
}

uint8_t stringToByte( char input[2] ){
    return (uint8_t)input[0];
}


void byte16ToString ( uint16_t input, char output[3]){
    if( isLittleEndian() ){ // little endian
        output[0] = input;
        output[1] = (input >> 8);
        output[2] = '\0';
    }else{
        output[0] = (input >> 8);
        output[1] = input;
        output[2] = '\0';
    }
}

uint16_t stringToByte16( char input[3] ){
    if( isLittleEndian() ){ // little endian
        return ((uint16_t)((uint8_t)input[1]) << 8) | ((uint8_t)input[0]);
    }else{
        return ((uint16_t)((uint8_t)input[0]) << 8) | ((uint8_t)input[1]);
    }
}


void byte32ToString ( uint32_t input, char output[5]){
    if( isLittleEndian() ){ // little endian
        output[3] = (input >> 24);
        output[2] = (input >> 16);
        output[1] = (input >> 8);
        output[0] = input;
        output[4] = '\0';
    }else{
        output[0] = (input >> 24);
        output[1] = (input >> 16);
        output[2] = (input >> 8);
        output[3] = input;
        output[4] = '\0';
    }
}

uint32_t stringToByte32( char input[5] ){
    if( isLittleEndian() ){ // little endian
        return ( (uint32_t)input[3] << 24 ) 
        | ((uint32_t)input[2] << 16) 
        | ((uint32_t)input[1] << 8) 
        | ((uint32_t)input[0]);
    }else{
        return ( (uint32_t)input[0] << 24 ) 
        | ((uint32_t)input[1] << 16) 
        | ((uint32_t)input[2] << 8) 
        | ((uint32_t)input[3]);
    }
}


void byte64ToString ( uint64_t input, char output[9]){
    if( isLittleEndian() ){ // little endian
        output[7] = (input >> 56);
        output[6] = (input >> 48);
        output[5] = (input >> 40);
        output[4] = (input >> 32);
        output[3] = (input >> 24);
        output[2] = (input >> 16);
        output[1] = (input >> 8);
        output[0] = input;
        output[8] = '\0';
    }else{
        output[0] = (input >> 56);
        output[1] = (input >> 48);
        output[2] = (input >> 40);
        output[3] = (input >> 32);
        output[4] = (input >> 24);
        output[5] = (input >> 16);
        output[6] = (input >> 8);
        output[7] = input;
        output[8] = '\0';
    }
}

uint64_t stringToByte64( char input[9] ){
    if( isLittleEndian() ){ // little endian
        return ((uint64_t)((uint64_t)input[7]) << 56) 
        | ((uint64_t)input[6] << 48) 
        | ((uint64_t)input[5] << 40)
        | ((uint64_t)input[4] << 32)  
        | ((uint64_t)input[3] << 24) 
        | ((uint64_t)input[2] << 16) 
        | ((uint64_t)input[1] << 8) 
        | ((uint64_t)input[0]);
    }else{
        return ((uint64_t)((uint64_t)input[0]) << 56) 
        | ((uint64_t)input[1] << 48) 
        | ((uint64_t)input[2] << 40)
        | ((uint64_t)input[3] << 32)  
        | ((uint64_t)input[4] << 24) 
        | ((uint64_t)input[5] << 16) 
        | ((uint64_t)input[6] << 8) 
        | ((uint64_t)input[7]);
    }
}


// Bytestream to Bytes

void byteToByteStream( uint8_t input, uint8_t output[1]){ output[0] = input; }

uint8_t byteStreamToByte( uint8_t input[1] ){ return input[0]; }

void byte16ToByteStream ( uint16_t input, uint8_t output[2]){
    if( isLittleEndian() ){ // little endian
        output[0] = input;
        output[1] = (input >> 8);
    }else{
        output[0] = (input >> 8);
        output[1] = input;
    }
}

uint16_t byteStreamToByte16( uint8_t input[2] ){
    if( isLittleEndian() ){ // little endian
        return ((uint16_t)(input[1]) << 8) | (input[0]);
    }else{
        return ((uint16_t)(input[0]) << 8) | (input[1]);
    }
}

void byte32ToBytestream ( uint32_t input, uint8_t output[4]){
    if( isLittleEndian() ){ // little endian
        output[3] = (input >> 24);
        output[2] = (input >> 16);
        output[1] = (input >> 8);
        output[0] = input;
    }else{
        output[0] = (input >> 24);
        output[1] = (input >> 16);
        output[2] = (input >> 8);
        output[3] = input;
    }    
}

uint32_t byteStreamToByte32( uint8_t input[4] ){
    if( isLittleEndian() ){ // little endian
        return ( (uint32_t)input[3] << 24 ) 
        | ((uint32_t)input[2] << 16) 
        | ((uint32_t)input[1] << 8) 
        | ((uint32_t)input[0]);
    }else{
        return ( (uint32_t)input[0] << 24 ) 
        | ((uint32_t)input[1] << 16) 
        | ((uint32_t)input[2] << 8) 
        | ((uint32_t)input[3]);
    }
}

void byte64ToByteStream ( uint64_t input, uint8_t output[8]){
    if( isLittleEndian() ){ // little endian
        output[7] = (input >> 56);
        output[6] = (input >> 48);
        output[5] = (input >> 40);
        output[4] = (input >> 32);
        output[3] = (input >> 24);
        output[2] = (input >> 16);
        output[1] = (input >> 8);
        output[0] = input;
    }else{
        output[0] = (input >> 56);
        output[1] = (input >> 48);
        output[2] = (input >> 40);
        output[3] = (input >> 32);
        output[4] = (input >> 24);
        output[5] = (input >> 16);
        output[6] = (input >> 8);
        output[7] = input;
    }
}

uint64_t byteStreamToByte64( uint8_t input[8] ){
    if( isLittleEndian() ){ // little endian
        return ((uint64_t)((uint64_t)input[7]) << 56) 
        | ((uint64_t)input[6] << 48) 
        | ((uint64_t)input[5] << 40)
        | ((uint64_t)input[4] << 32)  
        | ((uint64_t)input[3] << 24) 
        | ((uint64_t)input[2] << 16) 
        | ((uint64_t)input[1] << 8) 
        | ((uint64_t)input[0]);
    }else{
        return ((uint64_t)((uint64_t)input[0]) << 56) 
        | ((uint64_t)input[1] << 48) 
        | ((uint64_t)input[2] << 40)
        | ((uint64_t)input[3] << 32)  
        | ((uint64_t)input[4] << 24) 
        | ((uint64_t)input[5] << 16) 
        | ((uint64_t)input[6] << 8) 
        | ((uint64_t)input[7]);
    }
}