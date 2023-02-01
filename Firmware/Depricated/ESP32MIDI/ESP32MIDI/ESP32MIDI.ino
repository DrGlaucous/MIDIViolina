/*
 Name:    ESP32MIDI.ino
 Created: 5/6/2022 6:17:48 PM
 Author:  Dr. G
*/

//#include <MIDI.h>
#include <Control_Surface.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include "configuration.h"


unsigned long MillisecondTicks{};
unsigned long MicrosecondTicks{};
unsigned long LastMillisecondTicks{};//previous values
unsigned long LastMicrosecondTicks{};



//MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
HardwareSerialMIDI_Interface MIDI_SERIAL = Serial1;
BluetoothMIDI_Interface MIDI_BLE;


//TEST MIDI FUNCTIONS
bool NoteGot = false;
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
//END

// Custom MIDI callback that prints incoming messages.
struct MyMIDI_Callbacks : FineGrainedMIDI_Callbacks<MyMIDI_Callbacks> {
    // Note how this ^ name is identical to the argument used here ^

    // Function that is called whenever a MIDI Note Off message is received. (This is not important when playing notes of a fixed length (I.E a music box))
    void onNoteOff(Channel channel, uint8_t note, uint8_t velocity, Cable cable)
    {
        BlinkLED(10);
        MIDINoteList[note] = false;
        NoteGot = true;
    }

    // Function that is called whenever a MIDI Note On message is received.
    void onNoteOn(Channel channel, uint8_t note, uint8_t velocity, Cable cable)
    {
        
        BlinkLED(10);
        MIDINoteList[note] = (velocity? true:false);
        NoteGot = true;

    }

} callback;



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




//Function that is called whenever a MIDI Note On message is received.
void onNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {

    BlinkLED(10);
    MIDINoteList[pitch] = (velocity? true:false);
    NoteGot = true;

    //HandleNote(pitch, (velocity? true:false) );//set new note, release note if we get a running standard note
    //ShiftBack();//apply new settings to shift registers


}
//off message
void onNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    BlinkLED(10);
    MIDINoteList[pitch] = false;
    NoteGot = true;
    //HandleNote(pitch, false);
    //ShiftBack();

}


void setup()
{
    //prepare pins

    LightLED = false;
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, true);

    //tie callbacks to midi events
    //MIDI.setHandleNoteOn(onNoteOn);
    //MIDI.setHandleNoteOff(onNoteOff);//this was needed for debugging, but not for the final firmware
    //MIDI.begin();


    MIDI_SERIAL.begin(); // Initialize midi interface
    MIDI_SERIAL.setCallbacks(callback);


    //change MIDI pins to the correct ones on the board. note, these can only be changed after Serial1 has been started
    //also, not all boards may be able to redefine these pins, the ESP32 can.
    Serial1.setPins(RX_PIN, TX_PIN);//rx, tx


    LightLED = false;
    pinMode(LED_BUILTIN, OUTPUT);


    //debug comms
    Serial.begin(115200);


}


void loop() {

    GetTicks();

    BlinkLED(0);//process the blinking of the LED

    //MIDI.read();
    MIDI_SERIAL.update(); // Update the Control Surface

    //keep registers updated
    if( MillisecondTicks != LastMillisecondTicks &&
        MillisecondTicks % 10 == 0 &&
        NoteGot)
    {
        ParseRawNoteList();
        NoteGot = false;

        //ShiftBack();
    }



    //debug stuff   
    if( MillisecondTicks != LastMillisecondTicks &&
        MillisecondTicks % 1000 == 0)
    {
        BlinkLED(100);

    }
    




}
