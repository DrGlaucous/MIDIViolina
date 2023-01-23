/*
 Name:		ESP32MIDI.ino
 Created:	5/6/2022 6:17:48 PM
 Author:	Dr. G
*/

#include <MIDI.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include "main.h"
#include "configuration.h"
#include "MovementHandler.h"


unsigned long MillisecondTicks{};
unsigned long MicrosecondTicks{};
unsigned long LastMillisecondTicks{};//previous values
unsigned long LastMicrosecondTicks{};


//HardwareSerial SerialPort(1);
//HardwareSerial Serial1();

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);



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


//Function that is called whenever a MIDI Note On message is received.
void onNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {

    digitalWrite(LED_BUILTIN, true);


    HandleNote(pitch, true);//set new note
    ShiftBack();//apply new settings to shift registers


}
//off message
void onNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    digitalWrite(LED_BUILTIN, false);

    HandleNote(pitch, false);
    ShiftBack();

}


void setup()
{
    //prepare pins
    InitPinout();

    //tie callbacks to midi events
    MIDI.setHandleNoteOn(onNoteOn);
    MIDI.setHandleNoteOff(onNoteOff);//this was needed for debugging, but not for the final firmware
    MIDI.begin();

    //change MIDI pins to the correct ones on the board. note, these can only be changed after Serial1 has been started
    //also, not all boards may be able to redefine these pins, the ESP32 can.
    Serial1.setPins(RX_PIN, TX_PIN);//rx, tx


    LightLED = false;
    pinMode(LED_BUILTIN, OUTPUT);


    //debug comms
    Serial.begin(9600);


}


void loop() {

    GetTicks();


    //an effort to conserve cycles by only calling this once per millisecond
    if( MillisecondTicks != LastMillisecondTicks)//% 1000 == 0)
    {
        //MIDI.read();
    }


    MIDI.read();

    //debuggu stuff
    if( MillisecondTicks != LastMillisecondTicks &&
        MillisecondTicks % 1000 == 0)
    {
        LightLED = (LightLED? 0 : 1);
        //Serial.println(LightLED);//debug, the ESP32 doesn't like using this when the MIDI serial port is open for some reason... maybe it is the GPIO pins I'm using?
        digitalWrite(LED_BUILTIN, LightLED);
        HandleNote(1, LightLED);
        //digitalWrite(SHIFT_CLEAR_PIN, LightLED);
        ShiftBack();

    }




}

