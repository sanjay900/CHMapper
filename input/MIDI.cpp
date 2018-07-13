//
// Created by sanjay on 4/07/18.
//

#include "MIDI.hpp"
#include "ControllerException.hpp"
#include "Wiimote.hpp"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include "buttons_ref.h"

MIDI::MIDI(const std::string &lua_name, sol::table &lua_table): Controller(lua_name, "MIDI", lua_table) {
    this->lua_table = lua_table;
    this->lua_name = lua_name;
    this->name = name;
    int port = lua_table["device"];
    lua_table["name"] = lua_name;
    midiin = new RtMidiIn();
    unsigned int nPorts = midiin->getPortCount();
    if ( nPorts < port ) {
        std::cout << "No ports available!\n";
        return;
    }

    midiin->openPort(port);
}

bool MIDI::try_to_use_device(struct udev * udev, struct udev_device * device, sol::state &lua) {
    return false;
}
bool MIDI::try_disconnect(const std::string &sysname,sol::state *lua) {
    return false;
}
MIDI::~MIDI() {
    if(isValid()) {
        close(fd);
        fd = -1;
    }
}
void MIDI::tick(sol::state& lua) {
    if (!isValid()) return;
    double stamp = midiin->getMessage( &message );
    ulong nBytes = message.size();
    if (nBytes > 0) {
        auto func = lua["midi_in"];
        if (func != nullptr) {
            func(lua_table, message[0], message[1], message[2]);
        }
    }

}

bool MIDI::isValid() const {
    return midiin->isPortOpen();
}