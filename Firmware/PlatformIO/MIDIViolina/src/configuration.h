//Configuration
//this program was built using PlatformIO.
//with some tweaks, it COULD be built using the arduino IDE enviroment. All libraries used in this project are arduino


//this is not really needed, but I have it here for my personal reference. the arduino IDE doesn't like it here, but platformIO doesn't mind
#ifndef USING_STM32
// Note: this needs to match with the PIN_MAP array in board.cpp
//STM32 pin mapping
enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13,PA14,PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13,PB14,PB15,
	PC13, PC14,PC15
};
#endif



//the state of a pin when it is "high" or "clicked" (nothing actually uses this in the code, but its useful for debugging)
#define ON_STATE 0


//Midi IO pins, connected to the DIN breakout
#define RX_PIN GPIO_NUM_13
#define TX_PIN GPIO_NUM_12






//number of notes on the violin, each one relating to a solenoid
#define FINGER_COUNT 28

//number of strings, each one relating to one solenoid and an N20 motor, the "Bow"
#define STRING_COUNT 4


