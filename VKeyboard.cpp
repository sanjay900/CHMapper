#include <cstdint>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include "VKeyboard.hpp"
#include "buttons_ref.h"
#include "ControllerException.h"

VKeyboard::VKeyboard(const std::string &lua_name, sol::table &lua_table, sol::state& lua) : lua_table(lua_table), lua_name(lua_name) {
    std::string dev_name = "SanPIE Virtual Device " + lua_name;
    struct libevdev *dev;
    dev = libevdev_new();
    libevdev_set_name(dev, dev_name.c_str());
    for (unsigned int i = 0; i < KEY_MAX && i < BUTTONS_SIZE; i++) {
        libevdev_enable_event_code(dev, EV_KEY, i, nullptr);
        keyFlags.push_back(false);
        std::string name = libevdev_event_code_get_name(EV_KEY, i);
        name = name.substr(std::string("KEY_").length());
        lua_table[name] = lua.create_table_with("down", [&,i]() { send_key(i,true); }, "up", [&,i]() { send_key(i,false); }, "get", [&,i]() { return get_key(i); }, "set", [&,i](bool v) { send_key(i,v); });
    }
    int err = libevdev_uinput_create_from_device(dev,
                                                 LIBEVDEV_UINPUT_OPEN_MANAGED,
                                                 &uidev);
    if (err != 0) {
        throw ControllerException(
                strerror(-err) + std::string(": Failed creating virtual device ") + lua_name + ".");
    }

    std::cout << "Created virtual device " << lua_name << ".\n";
    lua_table["axis_max"] = MAX_ABS_VAL;
    lua_table["axis_min"] = MIN_ABS_VAL;
}

VKeyboard::~VKeyboard() {
    libevdev_uinput_destroy(uidev);
}


void VKeyboard::send_key(uint type, bool value) {
    if (get_key(type) == value)
        return;
    keyFlags[type] = value;
    libevdev_uinput_write_event(uidev, EV_KEY, type, value);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
}
