//
// Created by sanjay on 13/07/18.
//

#include "OutputFactory.hpp"
#include "VKeyboard.hpp"
#include "VMIDI.hpp"
#include "VJoy.hpp"
#include "SerialOut.h"

void OutputFactory::create(std::string &name, sol::table &lua_dev, sol::state &lua) {
    sol::optional<std::string> typeOpt = lua_dev["type"];
    std::string type;
    if (typeOpt != sol::nullopt) {
        type = typeOpt.value();
    }
    if (type == "keyboard") {
        lua_dev["dev"] = new VKeyboard(name, lua_dev, lua);
    } else if(type == "midi") {
        lua_dev["dev"] = new VMIDI(name, lua_dev, lua);
    } else if(type == "serial"){
        lua_dev["dev"] = new SerialOut(name, lua_dev);
    } else {
        lua_dev["dev"] = new VJoy(name, lua_dev);
    }

}
