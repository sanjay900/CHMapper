//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_MIDID_H
#define WEJOY_MIDID_H


#include "Input.hpp"
#include "CoreMIDIIn.hpp"
#include <rtmidi/RtMidi.h>

class MIDIIn: public CoreMIDIIn {
    RtMidiIn* midiin;
    std::vector<unsigned char> message;
public:
    MIDIIn(const std::string &name, sol::table &dev);
    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;
    bool try_disconnect(const std::string &sysname,sol::state *lua) override;
    void tick(sol::state &lua) override;

    void send_message(unsigned char *buf) override;

private:
    bool isValid() const override;

public:
    ~MIDIIn();

};


#endif
