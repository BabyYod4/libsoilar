#include "arduino_uno.h"

#if BOARD_TYPE == ARDUINO_UNO

    #include <SPI.h> 
    #include <Arduino.h>

    SPIClass spi(SPI);

    void gpioInit( gpio_t pin, int mode ){ pinMode( pin, mode ); }
    void gpioSet( gpio_t pin, int state ){ digitalWrite( pin, state ); }

    int spiInit( spi_t dev ){ spi.begin(); return dev; }
    int spiAquire( spi_t dev, spi_clk_t freq, spi_mode_t mode, spi_order_t order){ spi.beginTransaction( SPISettings(freq, order, mode) ); return dev; }
    int spiRelease( spi_t dev ){ spi.endTransaction(); return dev; }
    int spiDestroy( spi_t dev ){ spi.end(); return dev; }
    uint8_t spiTransfer( spi_t dev, uint8_t value ){ return spi.transfer(value); }

    void yieldThread(){ yield(); }
    void sleepMs(int ms){ delay(ms); }
    void deepSleep(){
        CLKPR = 0x80;
        set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Set the sleep mode
        sleep_enable();
        sleep_cpu(); // Go to sleep
    };

    void wakeUp(){
        CLKPR = 0x08;
        sleep_disable();
    }

    void addInterrupt( gpio_t pin, void (*callback)(void), int condition ) {
        #ifdef SPI_HAS_NOTUSINGINTERRUPT
        spi.usingInterrupt(digitalPinToInterrupt(pin));
        #endif
        attachInterrupt(digitalPinToInterrupt(pin), callback, condition);
    }

    void throwException( const char* what ){
        fprintf(stderr, what);
        exit(EXIT_FAILURE);
    }

    void serialInit( int baudRate ){
        Serial.begin(baudRate);
    }

    void print(const char* msg){
        Serial.print(msg);
    }

    void println(const char* msg){
        Serial.println(msg);
    }

    void printByte(uint8_t byte){
        Serial.print(byte);
    }

#endif //#if BOARD_TYPE == ARDUINO_UNO