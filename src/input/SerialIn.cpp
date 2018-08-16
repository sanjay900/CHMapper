//
// Created by sanjay on 14/07/18.
//

#include <src/DeviceException.hpp>
#include <fcntl.h>
#include <linux/serial.h>
#include <termio.h>
#include <zconf.h>
#include <chrono>
#include "SerialIn.hpp"
auto begin = std::chrono::high_resolution_clock::now();
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
SerialIn::SerialIn(const std::string &lua_name, sol::table &dev) : Serial(dev), Input(lua_name, "Serial", dev) {}

bool SerialIn::isValid() const {
    return Serial::isValid();
}

