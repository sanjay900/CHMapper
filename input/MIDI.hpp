//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_MIDIBASE_H
#define WEJOY_MIDIBASE_H


#include "Controller.hpp"
#include <rtmidi/RtMidi.h>

class MIDI: public Controller {
    friend class MIDIDirect;
    friend class MIDISerial;
    bool debug;
    MIDI(const std::string &lua_name, const std::string &name, sol::table &dev);

public:
    void parse_midi_command(unsigned char *buf, sol::state &lua);
};


#endif
