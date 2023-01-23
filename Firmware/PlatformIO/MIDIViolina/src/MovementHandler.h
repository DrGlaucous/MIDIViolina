

#pragma once

#include "configuration.h"

typedef struct FINGER_STRUCT
{
    int note;//MIDI note ID
    int stringID;//what string must also be pressed
    bool status;//On or off
}FINGER_STRUCT;

typedef struct STRING_STRUCT
{
    bool status;//solenoid status
    unsigned char speed;//motor speed
    int timeout;//idle time left until we turn the motor off
}STRING_STRUCT;

extern FINGER_STRUCT Fingers[FINGER_COUNT];
extern STRING_STRUCT StringStat[STRING_COUNT];


void InitPinout(void);
void HandleNote(int note, bool noteStatus);
void ShiftBack(void);


