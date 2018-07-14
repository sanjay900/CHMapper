//
// Created by sanjay on 13/07/18.
//

#include "InputFactory.hpp"
#include "MIDIDirect.hpp"
#include "MIDISerial.hpp"

Input* InputFactory::create(std::string &name, sol::table &lua_table) {
    sol::optional<std::string> typeOpt = lua_table["type"];
    if (typeOpt == sol::nullopt) {
        throw DeviceException("No type was defined.");
    }
    std::string type = typeOpt.value();
    if (type == "wii") {
        return new Wiimote(name, lua_table);
    }
    if (type == "midi") {
        return new MIDIDirect(name, lua_table);
    }
    if (type == "midi_serial") {
        return new MIDISerial(name, lua_table);
    }
    if (type == "serial") {
        return new Serial(name, lua_table);
    }
    return new Controller(name, type, lua_table);
}