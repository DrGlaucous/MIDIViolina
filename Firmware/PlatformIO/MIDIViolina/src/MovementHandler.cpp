
#include <Arduino.h>
#include "configuration.h"
#include "MovementHandler.h"


//define the pins for each finger
//Note, fingerID, status
FINGER_STRUCT Fingers[] = 
{
    {1, 0, false},
    {1, 0, false},
    {1, 0, false},
    {1, 0, false},
    {1, 0, false},
    {1, 0, false},
    {1, 0, false},

    {1, 1, false},
    {1, 1, false},
    {1, 1, false},
    {1, 1, false},
    {1, 1, false},
    {1, 1, false},
    {1, 1, false},

    {1, 2, false},
    {1, 2, false},
    {1, 2, false},
    {1, 2, false},
    {1, 2, false},
    {1, 2, false},
    {1, 2, false},

    {1, 3, false},
    {1, 3, false},
    {1, 3, false},
    {1, 3, false},
    {1, 3, false},
    {1, 3, false},
    {1, 3, false},
};

STRING_STRUCT StringStat[STRING_COUNT]
{
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
    {false, 0, 0},
};


//On Note Get:
//iterate through finger struct, set note to true or false, depending on midi message type
//check corresponding motor, if motor is off, set to on, and push it down
//if note off, remove solenoid power, but put motor on a timer, after timer expieres, turn motor off
//every X ticks, shift data into registers



void HandleNote(int note, bool noteStatus)
{

    for(int i = 0; i < NOTE_MAX; ++i)
        if(Fingers[i].note == note)
            {
                Fingers[i].status = noteStatus;

                if(noteStatus)
                {
                    StringStat[Fingers[i].stringID].status = noteStatus;
                    
                }
                else//need to check if there are any other notes using this string. if so, do not turn it off
                {
                    bool leaveOn = false;//if true, we do not disable the string solenoid

                    for(int p = 0; p < NOTE_MAX; ++p)
                        if(Fingers[p].stringID == Fingers[i].stringID &&
                        Fingers[p].status == true)
                        {
                            leaveOn = true;//found another note in use, do not turn off
                            break;//no more scanning needed
                        }

                    //no matching notes found, we are allowed to disable the string
                    if(!leaveOn)
                        StringStat[Fingers[i].stringID].status = noteStatus;

                }


            }


}








