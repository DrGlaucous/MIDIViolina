/*
 Name:		ESP32MIDI.ino
 Created:	5/6/2022 6:17:48 PM
 Author:	Dr. G
*/

//#include <MIDI.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include "main.h"
#include "configuration.h"
#include "MovementHandler.h"
#include <Control_Surface.h>



unsigned long MillisecondTicks{};
unsigned long MicrosecondTicks{};
unsigned long LastMillisecondTicks{};//previous values
unsigned long LastMicrosecondTicks{};


//old MIDI.h setup stuff, had problems catching all note events, so it was dropped in favor of control_surface
//MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);


HardwareSerialMIDI_Interface MIDI_SERIAL = Serial1;
//BluetoothMIDI_Interface MIDI_BLE;


//for debug
bool LightLED;

//System clock
void GetTicks(void)
{
	LastMillisecondTicks = MillisecondTicks;
	LastMicrosecondTicks = MicrosecondTicks;

	MillisecondTicks = millis();
	MicrosecondTicks = micros();
}

void BlinkLED(unsigned short time)
{
    //how much time until we turn the LED off
    static unsigned short timeout = 0;

    if(time != 0)
    {
        timeout = time;
    }
    else
        if(LastMillisecondTicks != MillisecondTicks &&
            timeout > 0)
            --timeout;


    if(timeout)
            digitalWrite(LED_BUILTIN, true);
        else
            digitalWrite(LED_BUILTIN, false);


}



bool NoteGot = false;//this is so we can save time and shift back new note events in larger sets, rather than 1x for every single event

//TEST MIDI FUNCTIONS (the old MIDI library would not catch all note events)
/*
bool MIDINoteList[0x100] = {};
void ParseRawNoteList(void)
{
    for(int i = 0; i < 0x100; ++i)
        if(MIDINoteList[i] == true)
            {
                Serial.printf("%d | ", i);
            }

    Serial.print("\n\r");

}
*/
//END



//controlSurface callback event
bool channelMessageCallback(ChannelMessage cm)
{

    uint8_t status = (cm.header & 0xF0) >> 4;//collect first 4 bits of data (event type)
    uint8_t channel = cm.header & 0x0F;//collect last 4 bits of data (note channel)
    uint8_t note = cm.data1;
    uint8_t velocity = cm.data2;

    //debug stuff: print the raw MIDI input and the semi-processed data
    //Serial.printf("Channel message: %02x, %02x, %02x \n\r", cm.header, cm.data1, cm.data2);
    //header layout: [event type x4][channel ID x4]
    //Serial.printf("Status: %01x | Channel: %01x | Note: %01x | Velocity: %01x  \n\r", status, channel, note, velocity);

    if(channel == MIDI_CHANNEL)
        switch(status)
        {
        case NOTE_OFF:
            {
                HandleNote(note, false);
                NoteGot = true;
                break;
            }
        case NOTE_ON:
            {
                HandleNote(note, (velocity? true:false));
                NoteGot = true;
                break;
            }
        }


    BlinkLED(10);

    return true;
    // Return true to indicate that handling is done,
    // and Control_Surface shouldn't handle it anymore.
    // If you want Control_Surface to handle it as well,
    // return false.

}



void setup()
{
    //prepare pins
    InitPinout();

    LightLED = false;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, true);//blink to show we've booted properly (sometimes it doesn't happen)


    //tie callbacks to midi events
    //MIDI.setHandleNoteOn(onNoteOn);
    //MIDI.setHandleNoteOff(onNoteOff);//this was needed for debugging, but not for the final firmware
    //MIDI.begin();


    Control_Surface.begin(); // Initialize midi interface
    Control_Surface.setMIDIInputCallbacks(channelMessageCallback, NULL, NULL);//set up callback function

    //change MIDI pins to the correct ones on the board. Note: these can only be changed after Serial1 has been started
    //also, not all boards may be able to redefine these pins, the ESP32 can.
    Serial1.setPins(RX_PIN, TX_PIN);//rx, tx




    //debug comms
    Serial.begin(9600);


}


void loop() {

    GetTicks();

    BlinkLED(0);//process the blinking of the LED

    //MIDI.read();
    MIDI_SERIAL.update(); // Update the Control Surface
    //MIDI_BLE.update();


    //keep registers updated
    if( MillisecondTicks != LastMillisecondTicks &&
        MillisecondTicks % 10 == 0 &&
        NoteGot)
    {
        NoteGot = false;
        ShiftBack();
    }



    //debug stuff   
    if( MillisecondTicks != LastMillisecondTicks &&
        MillisecondTicks % 1000 == 0)
    {
        //BlinkLED(100);
        if(CheckNote())
            BlinkLED(100);

        //LightLED = (LightLED? 0 : 1);
        //Serial.println(LightLED);//debug
        //digitalWrite(LED_BUILTIN, LightLED);
        //HandleNote(1, LightLED);
        //digitalWrite(SHIFT_CLEAR_PIN, LightLED);
        //ShiftBack();

    }
    




}

