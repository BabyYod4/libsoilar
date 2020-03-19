#ifndef IC880A_HPP
#define IC880A_HPP

#include <wiringPi.h>
#include <iostream>
#include <boost/chrono/chrono.hpp>
#include "../lora_gateway.hpp"
#include "../../libloragw/cpp_hal_adapter.h"

const GatewayChannelData iC880aChannel[] = { 
    { 0, 0, 0 }, // just used as index padding so it starts from 1 not 0...
    
    { 1, -187500,   1 }, // IF0 Radio B 
    { 2,  -62500,   1 }, // IF1 Radio B 
    { 3,   62500,   1 }, // IF2 Radio B 
    { 4,  187500,   1 }, // IF3 Radio B 
    
    { 5, -187500,   0 }, // IF4 Radio A 
    { 6,  -62500,   0 }, // IF5 Radio A 
    { 7,   62500,   0 }, // IF6 Radio A
    { 8,  187500,   0 }  // IF7 Radio A
};

class iC880a : public LoraGateway{

public:
    iC880a(const LoraGatewayDeviceGroups& deviceGroups);
    void init() override;
    void stop() override;
    void start() override;

    bool read() override;
    void send(const std::vector< LoraPackage >& msgList) override;

protected:
    void setRXmode() override;
    void setTXmode() override;

private:
    boost::mutex rxBuffMutex;
    boost::mutex txBuffMutex;

    lgw_pkt_rx_s rxBuff[8];
    lgw_pkt_tx_s txBuff[8];

    lgw_conf_board_s boardconf;
    lgw_conf_rxrf_s radioA; lgw_conf_rxrf_s radioB;
    lgw_conf_rxif_s ch1; lgw_conf_rxif_s ch2; lgw_conf_rxif_s ch3; lgw_conf_rxif_s ch4;
    lgw_conf_rxif_s ch5; lgw_conf_rxif_s ch6; lgw_conf_rxif_s ch7; lgw_conf_rxif_s ch8;
};

#endif //IC880A_HPP