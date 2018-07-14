//
// Created by sanjay on 4/07/18.
//
#include "Input.hpp"
#include "DeviceException.hpp"
#include "Wiimote.hpp"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include "output/buttons_ref.h"

Input::Input(const std::string &lua_name, const std::string &name, sol::table &lua_table) {
    this->lua_table = lua_table;
    this->lua_name = lua_name;
    this->name = name;
    lua_table["type"] = name;
    lua_table["name"] = lua_name;
}

const std::string &Input::getLua_name() const {
    return lua_name;
}

const std::string &Input::getName() const {
    return name;
}

bool operator<(const Input &lhs, const Input &rhs) {
    return lhs.lua_name < rhs.lua_name;
}

bool operator>(const Input &lhs, const Input &rhs) {
    return rhs < lhs;
}

bool operator<=(const Input &lhs, const Input &rhs) {
    return !(rhs < lhs);
}

bool operator>=(const Input &lhs, const Input &rhs) {
    return !(lhs < rhs);
}


bool Input::isValid() const {
    return fd != -1;
}
