//
// Created by sanjay on 15/07/18.
//

#include <zconf.h>
#include "SerialOut.h"

SerialOut::SerialOut(const std::string &lua_name, sol::table &lua_table): Serial(lua_table) {
    lua_table["write"] = [&](sol::table table) {
        std::vector<unsigned char> data;
        for (auto &a: table) {
            data.push_back(a.second.as<unsigned char>());
        }
        return write(fd, &data[0], data.size());
    };
}
