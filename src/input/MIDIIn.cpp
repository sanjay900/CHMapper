//
// Created by sanjay on 4/07/18.
//

#include "MIDIIn.hpp"
#include "src/DeviceException.hpp"
#include "Wiimote.hpp"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>

MIDIIn::MIDIIn(const std::string &lua_name, sol::table &lua_table): Input(lua_name, "MIDI", lua_table), CoreMIDIIn(lua_name, lua_table) {
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

bool MIDIIn::try_to_use_device(struct udev * udev, struct udev_device * device, sol::state &lua) {
    return false;
}
bool MIDIIn::try_disconnect(const std::string &sysname,sol::state *lua) {
    return false;
}
MIDIIn::~MIDIIn() {
    if(isValid()) {
        midiin->closePort();
    }
}
void MIDIIn::tick(sol::state& lua) {
    if (!isValid()) return;
    midiin->getMessage( &message );
    ulong nBytes = message.size();
    if (nBytes > 0) {
        parse_midi_command(&message[0], lua);
    }

}

bool MIDIIn::isValid() const {
    return midiin->isPortOpen();
}

void MIDIIn::send_message(unsigned char *buf) {

}
