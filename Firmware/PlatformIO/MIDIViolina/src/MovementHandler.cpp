
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

//set up pins and starting states
void InitPinout(void)
{

    //prep shift register pins
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(SHIFT_CLEAR_PIN, OUTPUT);
    pinMode(OUTPUT_ENABLE_PIN, OUTPUT);

    //make sure our output is enabled (follows reverse logic)
    digitalWrite(OUTPUT_ENABLE_PIN, LOW);
    digitalWrite(SHIFT_CLEAR_PIN, HIGH);//do not shift clear (set LOW to clear)


}

//update the proper table values based on note reception
void HandleNote(int note, bool noteStatus)
{

    for(int i = 0; i < FINGER_COUNT; ++i)//had NOTE_MAX, define mistake
        if(Fingers[i].note == note)//if this finger is our note
            {
                Fingers[i].status = noteStatus;

                if(noteStatus)
                {
                    StringStat[Fingers[i].stringID].status = noteStatus;
                    
                }
                else//need to check if there are any other notes using this string. if so, do not turn it off
                {
                    bool leaveOn = false;//if true, we do not disable the string solenoid

                    for(int p = 0; p < FINGER_COUNT; ++p)
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


void ShiftBack(void)
{
    bool RegisterArray[FINGER_COUNT + STRING_COUNT];

    //fingers are fed in first, then strings
    for(int i = 0; i < FINGER_COUNT; ++i)
        RegisterArray[i] = Fingers[i].status;
    for(int i = 0; i < STRING_COUNT; ++i)
        RegisterArray[FINGER_COUNT + i] = StringStat[i].status;

    //TEST print array
    Serial.print("\n START \n");
    for(int i = 0; i < FINGER_COUNT + STRING_COUNT; i++)
    {
        Serial.print(RegisterArray[i]);
    }
    Serial.print("\n FINISH \n");



    //we now have an array that is ready to be fed into the registers
    {
        digitalWrite(LATCH_PIN, LOW);//ready to get data
    
        //push all bits into the shift register
        for(int i = 0; i < FINGER_COUNT + STRING_COUNT; i++)
        {   
            digitalWrite(DATA_PIN, RegisterArray[i]);

            //advance register
            digitalWrite(CLOCK_PIN, HIGH);
            //delay(1);//for testing
            digitalWrite(CLOCK_PIN, LOW);
            //delay(1);
        }


        digitalWrite(LATCH_PIN, HIGH);//seal it up, we're done
    }



}





