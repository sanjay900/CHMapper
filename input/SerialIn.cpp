//
// Created by sanjay on 14/07/18.
//

#include <DeviceException.hpp>
#include <fcntl.h>
#include <linux/serial.h>
#include <termio.h>
#include <zconf.h>
#include "SerialIn.hpp"

bool SerialIn::try_to_use_device(struct udev *, struct udev_device *, sol::state &lua) {
    return false;
}

bool SerialIn::try_disconnect(const std::string &sysname, sol::state *lua) {
    return false;
}

void SerialIn::tick(sol::state &lua) {
    unsigned char buf;
    if (read(fd, &buf, 1) < 0) {
        return;
    }
    auto func = lua["serial_in"];
    if (func.valid()) {
        func(lua_table, buf);
    }
}
SerialIn::SerialIn(const std::string &lua_name, sol::table &dev) : Serial(dev), Input(lua_name, "Serial", dev) {
    sysname = lua_table["device"];
    lua_table["name"] = lua_name;
}

bool SerialIn::isValid() const {
    return Serial::isValid();
}

