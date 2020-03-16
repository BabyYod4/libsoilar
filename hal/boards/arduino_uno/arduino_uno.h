#ifndef ARDUINO_UNO_HPP
#define ARDUINO_UNO_HPP

#include "../../settings.h"

#if BOARD_TYPE == ARDUINO_UNO
        
    #include <stdlib.h>
    #include <stdio.h> 
    #include <stdint.h>
    #include <stdbool.h>
    #include <avr/sleep.h>

    #define INIT_ENVOIRMENT <Arduino.h>

    typedef int gpio_t;
    typedef int spi_t;
    typedef int spi_mode_t;
    typedef double spi_clk_t;
    typedef int spi_order_t;

    #define GPIO_OUT 0x1
    #define GPIO_IN 0x0

    #define HIGH 0x1
    #define LOW 0x0

    #define SPI_MODE_0 0x00
    #define SPI_MODE_1 0x04
    #define SPI_MODE_2 0x08
    #define SPI_MODE_3 0x0C

    #define SPI_ORDER_MSB 1
    #define SPI_ORDER_LSB 0

    #define SPI_CLK_8MHZ 8E6

    #define ON_RISE 3

    #define ISR_PREFIX
    #define ISR_CODE ISR_PREFIX

    void gpioInit( gpio_t pin, int mode );
    void gpioSet( gpio_t pin, int state );

    int spiInit( spi_t dev );
    int spiAquire( spi_t dev, spi_clk_t freq, spi_mode_t mode, spi_order_t order);
    int spiRelease( spi_t dev );
    int spiDestroy( spi_t dev );
    uint8_t spiTransfer( spi_t dev, uint8_t value );

    void yieldThread();
    void sleepMs(int ms);
    void deepSleep();
    void wakeUp();

    void addInterrupt( gpio_t pin, void (*callback)(void), int condition );
    void throwException( const char* what );

    void serialInit( int baudRate );
    void print(const char* msg);
    void println(const char* msg);
    void printByte(uint8_t byte);    

#endif //BOARD_TYPE == ARDUINO_UNO

#endif //ARDUINO_UNO_HPP
