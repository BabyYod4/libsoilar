#ifndef LUCIDY_HAL_H
#define LUCIDY_HAL_H

#include "settings.h"

#if LIB_TYPE == LIB_CPP
    #if BOARD_TYPE == ARDUINO_UNO
        #include "boards/arduino_uno/arduino_uno.h"            
    #endif //BOARD_TYPE_ARDUINO
#endif //LIB_TYPE_CPP

#if LIB_TYPE == LIB_C
        // ... Place C lib adapter code here ...
#endif //LIB_TYPE_C

#endif //LUCIDY_HAL_H