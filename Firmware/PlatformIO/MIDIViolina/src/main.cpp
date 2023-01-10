/*
 Name:		ESP32MIDI.ino
 Created:	5/6/2022 6:17:48 PM
 Author:	Dr. G
*/

#include <MIDI.h>
#include <Arduino.h>
#include <HardwareSerial.h>
//#include <Control_Surface.h>//an alternate MIDI library, but is not compatible with the STM32 (I originally inteded to use an ESP32)
#include "main.h"
#include "configuration.h"
//#include "ServoHandler.h"


unsigned long MillisecondTicks{};
unsigned long MicrosecondTicks{};
unsigned long LastMillisecondTicks{};//previous values
unsigned long LastMicrosecondTicks{};


//HardwareSerial SerialPort(1);
//HardwareSerial Serial1();

//MIDI_CREATE_INSTANCE(HardwareSerial, SerialPort, MIDI);

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


// Function that is called whenever a MIDI Note On message is received.
void onNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {

    digitalWrite(LED_BUILTIN, true);



}
//this was needed for debugging, but not for the final firmware
void onNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    digitalWrite(LED_BUILTIN, false);

}


void setup()
{

    //SerialPort.setPins(GPIO_NUM_11, GPIO_NUM_10);//rx, tx

    Serial1.setPins(GPIO_NUM_11, GPIO_NUM_10);//rx, tx


    //tie callbacks to midi events
    //MIDI.setHandleNoteOn(onNoteOn);
    //MIDI.setHandleNoteOff(onNoteOff);//this was needed for debugging, but not for the final firmware
    //MIDI.begin();



    LightLED = false;
    pinMode(LED_BUILTIN, OUTPUT);

    Serial1.begin(9600);

}


void loop() {

    GetTicks();


    //an effort to conserve cycles by only calling this once per millisecond
    if( MillisecondTicks != LastMillisecondTicks)//% 1000 == 0)
    {
        //MIDI.read();
    }


    if( MillisecondTicks != LastMillisecondTicks &&
        MillisecondTicks % 1000 == 0)
    {
        LightLED = (LightLED? 0 : 1);
        Serial1.println(LightLED);
        //digitalWrite(LED_BUILTIN, LightLED);
    }




}

