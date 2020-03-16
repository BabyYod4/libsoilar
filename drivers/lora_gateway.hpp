#ifndef LORA_GATEWAY_HPP
#define LORA_GATEWAY_HPP

#include <vector>
#include <array>
#include <inttypes.h>
#include <boost/thread/mutex.hpp>
#include <functional>

enum class LoraType{
    LORA_868_EU,
    LORA_915_US,
    LORA_433_ASIA
};

enum class LoraDeviceModes{
    TX_MODE,
    RX_MODE    
};

typedef struct ChannelData{ uint8_t channelID; int32_t offsetFreq; uint8_t radioNum; } ChannelData;

typedef struct LoraPackage{
    std::array<uint8_t, 256> data;
} LoraPackage;

class LoraGateway{

public:

    virtual void init() = 0;
    virtual void stop() = 0;
    virtual void start() = 0;

    virtual bool read() = 0;
    virtual void send() = 0;

    virtual void print() { printBuffer(-1, -1); }
    virtual void print(int min, int max) { printBuffer(min, max); }

protected:
    std::array< LoraPackage, 8 > gatewayBuffer;
    boost::mutex gatewayBufferMutex;

    bool claimBuffer( const std::function< bool( std::array< LoraPackage, 8 >& ) > & task){
        bool result = false;
        gatewayBufferMutex.lock();
        result = task(gatewayBuffer);
        gatewayBufferMutex.unlock();
        return result;
    }

    void printBuffer(int min, int max){
        gatewayBufferMutex.lock();
        if (min == -1 || max == -1){
            for (const LoraPackage& pkg: gatewayBuffer){
                for (const uint8_t& e : pkg.data) { std::cout << +e << "-";  }
                std::cout << std::endl << std::endl;
            }
        }else if(min > 0 && max < 256){
            for (const LoraPackage& pkg: gatewayBuffer){
                for (uint8_t i=min; i<max; i++) { std::cout << +pkg.data[i] << "-";  }
                std::cout << std::endl;
            }
        }
        gatewayBufferMutex.unlock();
    }


};

#endif //LORA_GATEWAY_HPP