//
// Created by sanjay on 4/07/18.
//
#include "Input.hpp"
#include "DeviceException.hpp"
#include "Controller.hpp"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include "output/buttons_ref.h"

Controller::Controller(const std::string &lua_name, const std::string &name, sol::table &lua_table): Input(lua_name, name, lua_table) {
    fd = -1;
}

void Controller::initMaps() {
    axisTypeBindings.clear();
    axisBindings.clear();
    buttonTypeBindings.clear();
    buttonBindings.clear();
    std::map<int, const char *>::iterator it;
    for (uint i = 0; i < ABS_MAX; i++) {
        if (libevdev_has_event_code(dev, EV_ABS, i)) {
            axisTypeBindings[i] = static_cast<int>(axisBindings.size());
            axisBindings.push_back(i);
        }
    }
    for (uint i = 0; i < KEY_MAX; i++) {
        if (libevdev_has_event_code(dev, EV_KEY, i)) {
            buttonTypeBindings[i] = static_cast<int>(buttonBindings.size());
            buttonBindings.push_back(i);
        }
    }
}

bool Controller::try_to_use_device(struct udev * udev, struct udev_device * device, sol::state &lua) {
    if (isValid()) {
        return udev_device_get_sysname(device) == sysname;
    }
    sysname = udev_device_get_sysname(device);
    const std::string devpath = "/dev/input/" + sysname;
    int fd = open(devpath.c_str(), O_RDONLY | O_NONBLOCK);
    struct libevdev *_dev = nullptr;
    int rc = libevdev_new_from_fd(fd, &_dev);
    if (rc < 0) {
        close(fd);
        return false;
    }
    const std::string found_name = libevdev_get_name(_dev);
    if (name.empty() || found_name == name) {
        this->dev = _dev;
        this->fd = fd;
        libevdev_grab(_dev, LIBEVDEV_GRAB);
        initMaps();
        if (name.empty()) {
            name = found_name;
        }
        return true;
    }
    sysname = "";
    close(fd);
    return false;
}

bool Controller::isValid() const {
    return fd != -1;
}

bool Controller::try_disconnect(const std::string &sysname,sol::state *lua) {
    if(isValid()) {
        if (sysname == this->sysname) {
            this->sysname = "";
            libevdev_free(dev);
            close(fd);
            fd = -1;
            if (lua != nullptr) {
                sol::state &s = *lua;
                auto func = s["disconnect_event"];
                if (func != nullptr) {
                    func(lua_table);
                }
            }
            return true;
        }
    }
    return false;
}
Controller::~Controller() {
    if(isValid()) {
        libevdev_free(dev);
        close(fd);
        fd = -1;
        sysname = "";
    }
}
int Controller::get_axis_min(uint type) {
    return libevdev_get_abs_minimum(dev, type);
}

int Controller::get_axis_max(uint type) {
    return libevdev_get_abs_maximum(dev, type);
}
void Controller::tick(sol::state& lua) {
    if (!isValid()) return;
    int rc;
    struct input_event ev{};
    rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
    if (rc == 0) {
        if (ev.type == EV_KEY) {
            auto func = lua["button_event"];
            if (func != nullptr) {
                func(lua_table, buttonTypeBindings[ev.code], ev.value==1);
            }
        }
        if (ev.type == EV_ABS) {
            auto func = lua["axis_event"];
            if (func != nullptr) {
                func(lua_table, axisTypeBindings[ev.code], scale(ev.value, get_axis_min(ev.code), get_axis_max(ev.code), MIN_ABS_VAL, MAX_ABS_VAL));
            }
        }
    }

}

int Controller::scale(int x, int in_min, int in_max, int out_min, int out_max) {
    return static_cast<int>(std::floor((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min));
}