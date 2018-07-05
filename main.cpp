#include <iostream>
#include <libudev.h>
#include "sol.hpp"
#include "VJoy.hpp"
#include "Controller.hpp"
#include "ControllerException.h"
#include <chrono>
#include <thread>

int main(int argc, char *argv[]) {
    sol::state lua;
    // open some common libraries
    lua.open_libraries(sol::lib::base, sol::lib::package);
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
        lua_dev["dev"] = Controller::create(name,lua_dev);
    }
    for (auto &device : v_devices) {
        auto name = device.first.as<std::string>();
        sol::table lua_dev = device.second;
        lua_dev["dev"] = new VJoy(name,lua_dev);
    }
    //TODO: we should probably disable the ability to have wii remotes without extensions when specific extensions are specified.
    //TODO: otherwise, disconnecting and connecting a controller would let remotes jump between devices, which doesnt really make any sense.
    udev_list_entry_foreach(dev_list_entry, device_list) {
        const char *path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);
        std::string dev_name = udev_device_get_sysname(dev);
        if (dev_name.find("event") != std::string::npos) {
            for (auto &device : devices) {
                sol::table lua_dev = device.second;
                Controller& c = lua_dev["dev"];
                if (c.try_to_use_device(udev, dev)) {
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
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lua.script("test()");
    }

    return 0;
}