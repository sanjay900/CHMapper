//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_MIDISER_H
#define WEJOY_MIDISER_H

#include <fstream>
#include <termios.h>
#include "Controller.hpp"
#include "MIDI.hpp"

#define MAX_MSG_SIZE 1024

class MIDISerial: MIDI {
    friend class InputFactory;
private:
    unsigned char buf[3];
    char msg[MAX_MSG_SIZE];
    struct termios oldtio, newtio;
    speed_t baudrate;
    MIDISerial(const std::string &name, sol::table &dev);
public:
    bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) override;
    bool try_disconnect(const std::string &sysname,sol::state *lua) override;
    void tick(sol::state &lua) override;
    ~MIDISerial() override;
};


#endif
