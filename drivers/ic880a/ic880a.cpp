#include "ic880a.hpp"

iC880a::iC880a(){
    _reset();
}

iC880a::iC880a(const LoraGatewayDeviceGroups& deviceGroups){ 
    _reset();
    setDeviceGroups(deviceGroups); 
    setSessionDeviceGroup(0);
}

void iC880a::init(){ 

/// ==== BOARD CONFIG ====    
    struct lgw_conf_board_s boardconf;
    boardconf.lorawan_public = false;
    boardconf.clksrc = 1; 
/// ==== ! BOARD CONFIG ====   


/// ==== RADIO CONFIG ====  
    struct lgw_conf_rxrf_s radioA;
    radioA.enable = true;
    radioA.freq_hz = (uint32_t)((867.0*1e6) + 0.5);
    radioA.rssi_offset = -169;
    radioA.type = LGW_RADIO_TYPE_SX1257;
    radioA.tx_enable = false;

    struct lgw_conf_rxrf_s radioB;
    radioB.enable = true;
    radioB.freq_hz = (uint32_t)((876.5*1e6) + 0.5);
    radioB.rssi_offset = -169;
    radioB.type = LGW_RADIO_TYPE_SX1257;
    radioB.tx_enable = false;
/// ==== ! RADIO CONFIG ====  


/// ==== CHANNEL CONFIG (1/2) ====  
    struct lgw_conf_rxif_s ch1;
    ch1.enable = true;
    ch1.rf_chain = iC880aChannel[1].radioNum;
    ch1.freq_hz = iC880aChannel[1].offsetFreq;
    ch1.bandwidth = BW_125KHZ;
    ch1.sync_word_size = 1;
    ch1.sync_word = 0x12;

    struct lgw_conf_rxif_s ch2;
    ch2.enable = true;
    ch2.rf_chain = iC880aChannel[2].radioNum;
    ch2.freq_hz = iC880aChannel[2].offsetFreq;
    ch2.bandwidth = BW_125KHZ;
    ch2.sync_word_size = 1;
    ch2.sync_word = 0x12;

    struct lgw_conf_rxif_s ch3;
    ch3.enable = true;
    ch3.rf_chain = iC880aChannel[3].radioNum;
    ch3.freq_hz = iC880aChannel[3].offsetFreq;
    ch3.bandwidth = BW_125KHZ;
    ch3.sync_word_size = 1;
    ch3.sync_word = 0x12;

    struct lgw_conf_rxif_s ch4;
    ch4.enable = true;
    ch4.rf_chain = iC880aChannel[4].radioNum;
    ch4.freq_hz = iC880aChannel[4].offsetFreq;
    ch4.bandwidth = BW_125KHZ;
    ch4.sync_word_size = 1;
    ch4.sync_word = 0x12;
/// ==== ! CHANNEL CONFIG (1/2) ====  


/// ==== CHANNEL CONFIG (2/2) ====
    struct lgw_conf_rxif_s ch5;
    ch5.enable = true;
    ch5.rf_chain = iC880aChannel[5].radioNum;
    ch5.freq_hz = iC880aChannel[5].offsetFreq;
    ch5.bandwidth = BW_125KHZ;
    ch5.sync_word_size = 1;
    ch5.sync_word = 0x12;

    struct lgw_conf_rxif_s ch6;
    ch6.enable = true;
    ch6.rf_chain = iC880aChannel[6].radioNum;
    ch6.freq_hz = iC880aChannel[6].offsetFreq;
    ch6.bandwidth = BW_125KHZ;
    ch6.sync_word_size = 1;
    ch6.sync_word = 0x12;

    struct lgw_conf_rxif_s ch7;
    ch7.enable = true;
    ch7.rf_chain = iC880aChannel[7].radioNum;
    ch7.freq_hz = iC880aChannel[7].offsetFreq;
    ch7.bandwidth = BW_125KHZ;
    ch7.sync_word_size = 1;
    ch7.sync_word = 0x12;

    struct lgw_conf_rxif_s ch8;
    ch8.enable = true;
    ch8.rf_chain = iC880aChannel[8].radioNum;
    ch8.freq_hz = iC880aChannel[8].offsetFreq;
    ch8.bandwidth = BW_125KHZ;
    ch8.sync_word_size = 1;
    ch8.sync_word = 0x12;
/// ==== ! CHANNEL CONFIG (2/2) ====
  
/// ==== APPLYING MODEM SETTINGS ====
    std::cout << "Configuring modem settings.... \r\n";
    
    _lgw_board_setconf( boardconf );

    _lgw_rxrf_setconf( 0, radioA); /* radio A, f0 */
    _lgw_rxrf_setconf( 1, radioB );

    _lgw_rxif_setconf( 0, ch1 );
    _lgw_rxif_setconf( 1, ch2 );
    _lgw_rxif_setconf( 2, ch3 );
    _lgw_rxif_setconf( 3, ch4 );

    _lgw_rxif_setconf( 4, ch5 );
    _lgw_rxif_setconf( 5, ch6 );
    _lgw_rxif_setconf( 6, ch7 );
    _lgw_rxif_setconf( 7, ch8 );

    std::cout << "Succesfully configured modem settings.... \r\n";
/// ==== ! APPLYING MODEM SETTINGS ====

}

void iC880a::stop(){ 
    _lgw_stop();
}

void iC880a::start(){ 
    _lgw_start();
}


bool iC880a::read(){
    _rxBuffMutex.lock();
    claimBuffer(

        [this](std::array< LoraPackage, 8 >& buff)->bool{
            int pktLen;
            uint8_t buffPointer = 0;

            uint8_t sessionDevNum = getSessionDeviceGroupWaitNum();
            uint16_t sessionTimeoutVal = getSessionDeviceGroupTimeout();
            uint16_t foundDevices[sessionDevNum] = {0};

            uint8_t devId_msb;
            uint8_t devId_lsb;
            uint16_t devId;
    
            bool allDevFound = false;
            bool timeout = false;
            
            std::function<bool(uint16_t)> isAlreadyfound = [&sessionDevNum, &foundDevices](uint16_t _devID)->bool{ 
                for (uint8_t i=0; i<sessionDevNum; i++){
                    if (_devID == foundDevices[i]){ return true; }
                }
                return false;
            };

            while ( !allDevFound ){
                pktLen = _lgw_receive(8, _rxBuff);
                
                if (pktLen > 0){

                    for( int i=0; i<pktLen; i++){

                        if( _rxBuff[i].payload[0] == 255 ){
                            devId_msb = _rxBuff[i].payload[1];
                            devId_lsb = _rxBuff[i].payload[2];
                            devId = ((uint16_t)devId_lsb << 8) | devId_msb;

                            if ( _rxBuff[i].payload[3] != 0 && isValidDevice(devId) && !isAlreadyfound(devId) ){
                                buff[buffPointer].devID = devId;
                                for(uint16_t pcol = 3; pcol < _rxBuff[i].size; pcol++ ){ buff[buffPointer].data[pcol-3] = _rxBuff[i].payload[pcol]; }
                                foundDevices[buffPointer] = devId;
                                buffPointer++;
                            }

                        }

                    }
                }

                if ( buffPointer >= sessionDevNum ){ allDevFound = true; }

            }

            return allDevFound;
        }
    );

    _rxBuffMutex.unlock();
    return true;
}


void iC880a::send(){

}

void iC880a::_reset(){ 
    pinMode( 9, OUTPUT );
    digitalWrite( 9, HIGH);
    delay(5000);
    digitalWrite(9, LOW);
 }