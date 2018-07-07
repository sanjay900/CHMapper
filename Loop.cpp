//
// Created by sanjay on 7/07/18.
//

#include <libudev.h>
#include <cstdio>
#include <string>
#include <chrono>
#include <thread>
#include "Loop.h"
#include "sol.hpp"
#include "Controller.hpp"

void Loop::tick() {
    long last = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
    struct udev* udev = udev_new();
    if (!udev) {
        fprintf(stderr, "udev_new() failed\n");
        return;
    }
    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");

    udev_monitor_filter_add_match_subsystem_devtype(mon, "input", NULL);
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);
    sol::table devices = lua["devices"];
    fd_set fds;
    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = 0;
    while(running) {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        if (select(fd+1, &fds, NULL, NULL, &tm) > 0) {
            if (FD_ISSET(fd, &fds)) {
                struct udev_device *dev = udev_monitor_receive_device(mon);
                const std::string sysname = udev_device_get_sysname(dev);
                const std::string action = udev_device_get_action(dev);
                for (auto &device : devices) {
                    sol::table lua_dev = device.second;
                    Controller &c = lua_dev["dev"];
                    if (action == "add") {
                        if (c.try_to_use_device(udev, dev, lua)) {
                            break;
                        }
                    } else if (action == "remove") {
                        c.try_disconnect(sysname);
                    }
                }
            }
        }
        long current = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
        lua["tick"](current-last);
        last = current;
        for (auto &device : devices) {
            auto name = device.first.as<std::string>();
            sol::table lua_dev = device.second;
            Controller& c = lua_dev["dev"];
            c.tick(lua);
        }
    }
}
void Loop::stop() {
    running = false;
    tickThread.join();
}