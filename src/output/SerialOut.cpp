//
// Created by sanjay on 15/07/18.
//

#include <zconf.h>
#include "SerialOut.h"

SerialOut::SerialOut(const std::string &lua_name, sol::table &lua_table): Serial(lua_table) {
    lua_table["write"] = [&](sol::table table) {
        std::vector<unsigned char> data;
        for (auto &a: table) {
            if (a.second.is<bool>()) {
                data.push_back(a.second.as<bool>());
            } else {
                data.push_back(a.second.as<unsigned char>());
            }
        }
        return write(fd, &data[0], data.size());
    };
}

bool SerialOut::try_to_use_device(struct udev *udev, struct udev_device *device, sol::state &lua) {
    return Serial::try_to_use_device(udev, device, lua);
}

bool SerialOut::try_disconnect(const std::string &sysname, sol::state *lua) {
    return Serial::try_disconnect(sysname, lua);
}
