//
// Created by sanjay on 5/07/18.
//

#include <cstdint>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include "VJoy.hpp"
#include "buttons_ref.h"
#include "ControllerException.h"

VJoy::VJoy(const std::string &lua_name, sol::table &lua_table) : lua_table(lua_table), lua_name(lua_name) {
    sol::optional<uint> b = lua_table["buttons"];
    if (b == sol::nullopt) {
        throw ControllerException("No button count was defined.");
    }
    buttons = b.value();
    sol::optional<uint> a = lua_table["axes"];
    if (a == sol::nullopt) {
        throw ControllerException("No axes count was defined.");
    }
    axes = a.value();
    std::cout << "Opening virtual device " << lua_name << ".\n";

    //Setup buttons for device
    if (buttons >= BUTTONS_SIZE)
        throw ControllerException(
                std::string("Number of buttons (") + std::to_string(axes) + ") for virtual device " + lua_name +
                " exceeds maximum allowable buttonss which is " + std::to_string(BUTTONS_SIZE - 1) + ".");

    //Setup axesMapping for device

    if (axes >= AXES_SIZE)
        throw ControllerException(
                std::string("Number of axes (") + std::to_string(axes) + ") for virtual device " + lua_name +
                " exceeds maximum allowable axesMapping which is " + std::to_string(AXES_SIZE - 1) + ".");
    axesData.resize(axes, 0);
    std::string dev_name = "WeJoy Virtual Device " + lua_name;
    struct libevdev *dev;
    dev = libevdev_new();
    libevdev_set_name(dev, dev_name.c_str());
    for (unsigned int i = 0; i < buttons && i < BUTTONS_SIZE; i++) {
        libevdev_enable_event_code(dev, EV_KEY, buttons_ref::BUTTONS[i], nullptr);
    }
    for (unsigned int i = 0; i < axes && i < AXES_SIZE; i++) {
        struct input_absinfo absinfo{};
        absinfo.maximum = MAX_ABS_VAL;
        absinfo.minimum = MIN_ABS_VAL;
        libevdev_enable_event_code(dev, EV_ABS, buttons_ref::AXES[i], &absinfo);
    }
    std::cout << "Creating virtual device " << lua_name << ".\n";
    int err = libevdev_uinput_create_from_device(dev,
                                                 LIBEVDEV_UINPUT_OPEN_MANAGED,
                                                 &uidev);
    if (err != 0) {
        throw ControllerException(
                strerror(-err) + std::string(": Failed creating virtual device ") + lua_name + ".");
    }

    std::cout << "Successfully created virtual device " << lua_name << ".\n";
    lua_table["send_button"] = [&](uint type, bool value) { return send_button_event(type, value); };
    lua_table["send_axis"] = [&](uint type, uint16_t value) { return send_axis_event(type, value); };
    lua_table["get_button"] = [&](uint type) { return get_button_status(type); };
    lua_table["get_axis"] = [&](uint type) { return get_axis_status(type); };
    lua_table["axis_max"] = MAX_ABS_VAL;
    lua_table["axis_min"] = MIN_ABS_VAL;
}

VJoy::~VJoy() {
    libevdev_uinput_destroy(uidev);
}

bool VJoy::get_button_status(int type) {
    return (buttonFlags & (1ul << type)) != 0;
}


void VJoy::send_button_event(uint type, bool value) {
    uint64_t check = (value) ? (buttonFlags | 1ul << type) : (buttonFlags & ~(1ul << type));
    if (check == buttonFlags) return;
    buttonFlags = check;
    libevdev_uinput_write_event(uidev, EV_KEY, buttons_ref::BUTTONS[type], value);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
}

void VJoy::send_axis_event(uint type, uint16_t value) {
    axesData[type] = value;
    libevdev_uinput_write_event(uidev, EV_ABS, buttons_ref::AXES[type], value);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
}
