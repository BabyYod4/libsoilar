// Specify which lib to use
#define ARDUINO_UNO               0
#define ARDUINO_DUE               1
#define BOARD_TYPE ARDUINO_UNO

// Specify lib type
#define LIB_C                       0
#define LIB_CPP                     1
#define LIB_TYPE LIB_CPP

// Specify wether driver code needs to optimize for low performance Micro Controllers. 
// Enabling this option may result in some functionality missing, but the core mechanism of the API will still work. 
#define LOW_PERFORMANCE             0
#define NORMAL                      1
#define MODE_TYPE LOW_PERFORMANCE