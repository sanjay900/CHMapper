#include <cstdint>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include <fcntl.h>
#include <zconf.h>
#include "VMIDI.hpp"
#include "buttons_ref.h"
#include "ControllerException.hpp"
#include "input/Controller.hpp"

VMIDI::VMIDI(const std::string &lua_name, sol::table &lua_table, sol::state& lua) : lua_table(lua_table), lua_name(lua_name) {

    std::string device = lua_table["device"];

    // step 1: open the OSS device for writing
    fd = open(device.c_str(), O_WRONLY, 0);
    if (fd < 0) {
        printf("Error: cannot open %s\n", device);
        exit(1);
    }
    lua_table["note"] = [&](unsigned char chan, unsigned char note, unsigned char velocity) {
        unsigned char data[3] = {static_cast<unsigned char>(0x90 + chan), note, velocity};

        // step 2: write the MIDI information to the OSS device
        write(fd, data, sizeof(data));
    };
    lua_table["note_off"] = [&](unsigned char chan, unsigned char note, unsigned char velocity) {
        unsigned char data[3] = {static_cast<unsigned char>(0x80 + chan), note, velocity};

        // step 2: write the MIDI information to the OSS device
        write(fd, data, sizeof(data));
    };
    lua_table["write"] = [&](unsigned char param1, unsigned char param2, unsigned char param3) {
        unsigned char data[3] = {param1, param2, param3};
        write(fd, data, sizeof(data));
    };

}

VMIDI::~VMIDI() {
    close(fd);
}
