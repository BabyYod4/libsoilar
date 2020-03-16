/**
 * ___________________________________
 * | Type  | WiringPi | Rpi | iC880a |
 * |-------|----------|-----|--------|
 * | GND1  | 6        | 6   | 12     |                 
 * | Reset | 9        | 5   | 13     |
 * | CLK   | 14       | 23  | 14     |
 * | MISO  | 13       | 21  | 15     |
 * | MOSI  | 12       | 19  | 16     |
 * | NSS   | 10       | 24  | 17     |
 * ===================================
 */

/**
 * LEDS:
 * 1) Backhaul packes 
 * 2) TX packet 
 * 3) RX Sensor packet 
 * 4) RX FSK packets 
 * 5) RX buffer not empty 
 * 6) Power
 */

#include "drivers/ic880a/ic880a.hpp"
#include <signal.h>        /* sigaction */

static int exit_sig = 0; /* 1 -> application terminates cleanly (shut down hardware, close open files, etc) */
static int quit_sig = 0; /* 1 -> application terminates without shutting down the hardware */

static void sig_handler(int sigio);
static void sig_handler(int sigio) {
    if (sigio == SIGQUIT) {
        quit_sig = 1;;
    } else if ((sigio == SIGINT) || (sigio == SIGTERM)) {
        exit_sig = 1;
    }
}

int main( void ) {
  wiringPiSetup();

  struct sigaction sigact; /* SIGQUIT&SIGINT&SIGTERM signal handling */
  /* configure signal handling */
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags = 0;
  sigact.sa_handler = sig_handler;
  sigaction(SIGQUIT, &sigact, NULL);
  sigaction(SIGINT, &sigact, NULL);
  sigaction(SIGTERM, &sigact, NULL);
 
  iC880a gateway;
  gateway.init();
  gateway.start();
  
  bool received = 0;
  while ((quit_sig != 1) && (exit_sig != 1)) {
    received = gateway.read();
    if (received){
      gateway.print(1, 5);
      std::cout << std::endl; 
    }

    delay(300);
  }

  printf("Test complete. Closing Gateway... \r\n");
  gateway.stop();
  printf( "Gateway closed! \r\n" );

  return 0;
}


