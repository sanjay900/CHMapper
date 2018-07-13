//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_MIDID_H
#define WEJOY_MIDID_H


#include "Controller.hpp"
#include "MIDI.hpp"
#include <rtmidi/RtMidi.h>

class MIDIDirect: MIDI {
    friend class InputFactory;
    friend class MIDI;
    MIDIDirect(const std::string &name, sol::table &dev);
    RtMidiIn* midiin;
    std::vector<unsigned char> message;
public:
    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;
    bool try_disconnect(const std::string &sysname,sol::state *lua) override;
    void tick(sol::state &lua) override;

private:
    bool isValid() const override;

public:
    ~MIDIDirect() override;

};


#endif
