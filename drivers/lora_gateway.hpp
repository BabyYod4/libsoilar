#ifndef LORA_GATEWAY_HPP
#define LORA_GATEWAY_HPP

#include <vector>
#include <array>
#include <inttypes.h>
#include <boost/thread/mutex.hpp>
#include <functional>

enum class LoraGatewayType{
    LORA_868_EU,
    LORA_915_US,
    LORA_433_ASIA
};

enum class LoraGatewayModes{
    TX_MODE,
    RX_MODE    
};

typedef std::vector< std::array<uint16_t, 9> > LoraGatewayDeviceGroups; // 0 = GroupSessionTimeoutID -- 1-8 = devId's,

typedef struct GatewayChannelData{ uint8_t channelID; int32_t offsetFreq; uint8_t radioNum; } GatewayChannelData;

typedef struct LoraPackage{
    uint16_t devID;
    std::array<uint8_t, 256> data;
} LoraPackage;

class LoraGateway{

public:

    virtual void init() = 0;
    virtual void stop() = 0;
    virtual void start() = 0;

    virtual bool read() = 0;
    virtual void send() = 0;

    virtual void print() { printBuffer(-1, -1, -1, -1); }
    virtual void print(int rowMin, int rowMax){ printBuffer(rowMin, rowMax, -1, -1); }
    virtual void print(int rowMin, int rowMax, int colMin, int colMax) { printBuffer(rowMin, rowMax, colMin, colMax); }

protected:
    std::array< LoraPackage, 8 > gatewayBuffer;
    boost::mutex gatewayBufferMutex;

    LoraGatewayDeviceGroups deviceGroups;
    uint16_t sessionDeviceGroup;
    boost::mutex deviceGroupsMutex;

    bool claimBuffer( const std::function< bool( std::array< LoraPackage, 8 >& ) > & task){
        bool result = false;
        gatewayBufferMutex.lock();
        result = task(gatewayBuffer);
        gatewayBufferMutex.unlock();
        return result;
    }

    void printBuffer(int rowMin, int rowMax, int colMin, int colMax){
        gatewayBufferMutex.lock();
        if (rowMin == -1){ rowMin=0; }
        if (rowMax == -1){ rowMax=8; }
        if (colMin == -1){ colMin=0; }
        if (colMax == -1){ colMax=255; }

       if(rowMin >= 0 && rowMax <= 8 && colMin >=0 && colMax <= 255){
            for(int row=rowMin; row<rowMax; row++){
                std::cout << "Endnode [" << +gatewayBuffer[row].devID << "] ";
                for(int col=colMin; col<colMax; col++){
                     std::cout << +gatewayBuffer[row].data[col] << "-";
                }
                std::cout << std::endl;
            }
        }
        gatewayBufferMutex.unlock();
    }

    void setDeviceGroups(const LoraGatewayDeviceGroups& newDeviceGroups){
        deviceGroupsMutex.lock();
        deviceGroups = newDeviceGroups;
        deviceGroupsMutex.unlock();
    }

    void setSessionDeviceGroup( uint16_t newSessionDeviceGroup ){
        deviceGroupsMutex.lock();
        sessionDeviceGroup = newSessionDeviceGroup;
        deviceGroupsMutex.unlock();
    }

    bool isValidDevice( uint16_t deviceID ){
        bool result = false;
        deviceGroupsMutex.lock();
        for(uint8_t i=1; i<9; i++){
            if ( deviceID == deviceGroups[sessionDeviceGroup][i] ) { result = true; break; }
        }
        deviceGroupsMutex.unlock();
        return result;
    }

    uint8_t getSessionDeviceGroupWaitNum(){
        uint8_t result = 0;
        deviceGroupsMutex.lock();
        for(uint8_t i=1; i<9; i++){
            if ( deviceGroups[sessionDeviceGroup][i] != 0 ) { result++; }
        }
        deviceGroupsMutex.unlock();
        return result;
    }

    uint16_t getSessionDeviceGroupTimeout(){
        uint16_t timeoutVal;
        deviceGroupsMutex.lock();
        timeoutVal = deviceGroups[sessionDeviceGroup][0];
        deviceGroupsMutex.unlock();
        return timeoutVal;
    }

};

#endif //LORA_GATEWAY_HPP