//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_MIDISER_H
#define WEJOY_MIDISER_H

#include <fstream>
#include "Input.hpp"
#include "CoreMIDIIn.hpp"
#include "SerialIn.hpp"

#define MAX_MSG_SIZE 1024

class MIDISerialIn: public CoreMIDIIn, public SerialIn {
private:
    unsigned int i = 1;
    unsigned char buf[3];
    char msg[MAX_MSG_SIZE];
public:
    MIDISerialIn(const std::string &name, sol::table &dev);
    void tick(sol::state &lua) override;

    bool try_to_use_device(struct udev *udev, struct udev_device *device, sol::state &lua) override;
};


#endif
