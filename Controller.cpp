//
// Created by sanjay on 4/07/18.
//

#include "Controller.hpp"
#include "ControllerException.h"
#include "Wiimote.h"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>

Controller::Controller(const std::string &lua_name, const std::string &name, sol::table &lua_table) {
    this->lua_table = lua_table;
    this->lua_name = lua_name;
    this->name = name;
}

Controller* Controller::create(std::string &name, sol::table &lua_table) {
    sol::optional<std::string> typeOpt = lua_table["type"];
    if (typeOpt == sol::nullopt) {
        throw ControllerException("No type was defined.");
    }
    std::string type = typeOpt.value();
    if (type == "Wii") {
        return new Wiimote(name, "Nintendo Wii Remote", lua_table);
    }
    return new Controller(name, type, lua_table);
}

const std::string &Controller::getLua_name() const {
    return lua_name;
}

const std::string &Controller::getName() const {
    return name;
}
void Controller::initMaps() {
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

bool Controller::try_to_use_device(struct udev * udev, struct udev_device * device) {
    if (isValid()) return false;
    const std::string devpath = "/dev/input/" + std::string(udev_device_get_sysname(device));
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
    close(fd);
    return false;
}

bool Controller::isValid() const {
    return fd == -1;
}

void Controller::disconnect() {
    if (isValid()) {
        libevdev_free(dev);
        close(fd);
    }

}