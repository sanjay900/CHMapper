//
// Created by sanjay on 4/07/18.
//

#include "Controller.hpp"
#include "ControllerException.h"
#include "Wiimote.h"
#include <utility>
#include <libudev.h>

Controller::Controller(const std::string &lua_name, const std::string &name, sol::table &lua_table) {
    this->lua_table = lua_table;
    this->lua_name = lua_name;
    this->name = name;
    lua_table["dev"] = this;
}

Controller* Controller::create(std::string &name, sol::table &lua_table) {
    sol::optional<std::string> typeOpt = lua_table["type"];
    if (typeOpt == sol::nullopt) {
        throw ControllerException("No type was defined.");
    }
    std::string type = typeOpt.value();
    if (type.find("Nintendo Wii Remote") != std::string::npos) {
        return new Wiimote(name, type, lua_table);
    }
    return new Controller(name, type, lua_table);
}

const std::string &Controller::getLua_name() const {
    return lua_name;
}

const std::string &Controller::getName() const {
    return name;
}

bool Controller::try_to_use_device(struct udev *, struct udev_device *) {

    return false;
}

bool Controller::isValid() const {
    return valid;
}
