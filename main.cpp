#include <iostream>
#include <libudev.h>
#include "sol.hpp"
#include "VJoy.hpp"
#include "VKeyboard.hpp"
#include "Controller.hpp"
#include "MIDI.hpp"
#include "ControllerException.h"
#include <chrono>
#include <thread>
#include <Loop.h>
#include <fcntl.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    sol::state lua;
    // open some common libraries
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::io, sol::lib::math);
    if (argc < 0) {
        std::cerr << "A script is required as the first argument" << std::endl;
    }
    lua.script_file(argv[1]);

    struct udev *udev;
    struct udev_device *dev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *device_list, *dev_list_entry;

    /* create udev object */
    udev = udev_new();
    if (!udev) {
        throw ControllerException("Cannot create udev context.");
    }

    /* create enumerate object */
    enumerate = udev_enumerate_new(udev);
    if (!enumerate) {
        throw ControllerException("Cannot create enumerate context.");
    }

    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);

    /* fillup device list */
    device_list = udev_enumerate_get_list_entry(enumerate);
    if (!device_list) {
        throw ControllerException("Failed to get device list.");
    }
    sol::table devices = lua["devices"];
    sol::table v_devices = lua["v_devices"];
    for (auto &device : devices) {
        auto name = device.first.as<std::string>();
        sol::table lua_dev = device.second;
        lua_dev["name"] = name;
        lua_dev["dev"] = Controller::create(name,lua_dev);
    }
    for (auto &device : v_devices) {
        auto name = device.first.as<std::string>();
        sol::table lua_dev = device.second;
        sol::optional<std::string> typeOpt = lua_dev["type"];
        std::string type = "";
        if (typeOpt != sol::nullopt) {
            type = typeOpt.value();
        }
        if (type == "keyboard") {
            lua_dev["dev"] = new VKeyboard(name, lua_dev, lua);
        } else if(type == "midi") {
            lua_dev["dev"] = new MIDI(name, lua_dev, lua);
        } else {
            lua_dev["dev"] = new VJoy(name, lua_dev);
        }
    }
    udev_list_entry_foreach(dev_list_entry, device_list) {
        const char *path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);
        std::string dev_name = udev_device_get_sysname(dev);
        if (dev_name.find("event") != std::string::npos) {
            for (auto &device : devices) {
                sol::table lua_dev = device.second;
                Controller& c = lua_dev["dev"];
                if (c.try_to_use_device(udev, dev,lua)) {
                    break;
                }
            }
        }
        udev_device_unref(dev);

    }
    /* free enumerate */
    udev_enumerate_unref(enumerate);
    /* free udev */
    udev_unref(udev);
    bool running = true;
    Loop loop = Loop(lua);
    std::cout << "Press 'q' and then 'ENTER' to quit!\n";
    while (getchar() != 'q');
    loop.stop();
    sleep(1);
    return 0;
}