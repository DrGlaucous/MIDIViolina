#pragma once



enum MIDI_EVENTS
{
    NOTE_OFF = 0x08,
    NOTE_ON = 0x09,
    KEY_AFTERTOUCH = 0x0A,
    CONTROL_CHANGE = 0x0B,
    PROGRAM_CHANGE = 0x0C,
    CHANNEL_AFTERTOUCH = 0x0D,
    PITCH_BEND = 0x0E,

};



extern unsigned long MillisecondTicks;
extern unsigned long MicrosecondTicks;
extern unsigned long LastMillisecondTicks;
extern unsigned long LastMicrosecondTicks;
