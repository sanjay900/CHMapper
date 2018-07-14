//
// Created by sanjay on 4/07/18.
//

#include "MIDIDirect.hpp"
#include "DeviceException.hpp"
#include "Wiimote.hpp"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include "output/buttons_ref.h"

MIDIDirect::MIDIDirect(const std::string &lua_name, sol::table &lua_table): Input(lua_name, "MIDI", lua_table), MIDI(lua_name, lua_table) {
    lua_table["name"] = lua_name;
    midiin = new RtMidiIn();
    unsigned int nPorts = midiin->getPortCount();
    for (unsigned int i =0; i < nPorts; i++) {
        if (sysname == midiin->getPortName(i)) {
            midiin->openPort(i);
            return;
        }
    }
    std::cout << "Unable to find MIDI device by name: " <<sysname<<". Found MIDI devices:" << std::endl;

    for (unsigned int i =0; i < nPorts; i++) {
        std::cout << midiin->getPortName(i)<< std::endl;
    }
}

bool MIDIDirect::try_to_use_device(struct udev * udev, struct udev_device * device, sol::state &lua) {
    return false;
}
bool MIDIDirect::try_disconnect(const std::string &sysname,sol::state *lua) {
    return false;
}
MIDIDirect::~MIDIDirect() {
    if(isValid()) {
        midiin->closePort();
    }
}
void MIDIDirect::tick(sol::state& lua) {
    if (!isValid()) return;
    midiin->getMessage( &message );
    ulong nBytes = message.size();
    if (nBytes > 0) {
        parse_midi_command(&message[0], lua);
    }

}

bool MIDIDirect::isValid() const {
    return midiin->isPortOpen();
}