
#include <Arduino.h>
#include "configuration.h"
#include "MovementHandler.h"


//define the pins for each finger
FINGER_STRUCT Fingers[FINGER_COUNT] = 
{
    {1, 2, false},
    {1, 2}

}



//On Note Get:
//iterate through finger struct, set note to true or false, depending on midi message type
//check corresponding motor, if motor is off, set to on, and push it down
//if note off, remove solenoid power, but put motor on a timer, after timer expieres, turn motor off
//every X ticks, shift data into registers



