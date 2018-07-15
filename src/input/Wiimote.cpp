//
// Created by sanjay on 4/07/18.
//

#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <zconf.h>
#include <src/sol.hpp>
#include "src/DeviceException.hpp"
#include "Wiimote.hpp"

Wiimote::Wiimote(const std::string &name, sol::table &lua_table): Controller(name, "Nintendo Wii Remote", lua_table) {
    sol::optional<std::string> ext = lua_table["extension_type"];
    lua_table["type"] = "Remote";
    if (ext != sol::nullopt) {
        extension_name = ext.value();
    }
}


bool Wiimote::try_to_use_device(struct udev * udev, struct udev_device * udev_device, sol::state &lua) {
    if (isValid()) {
        if (!extension->isValid()) {
            struct udev_enumerate *enumerate = udev_enumerate_new(udev);
            //Find the parent wiimote hid device, and then start a search of its child devices
            udev_enumerate_add_match_parent(enumerate, udev_device_get_parent_with_subsystem_devtype(udev_device,"hid",nullptr));
            udev_enumerate_add_match_subsystem(enumerate, "input");
            udev_enumerate_scan_devices(enumerate);

            struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
            struct udev_list_entry *entry;
            struct udev_device *entry_dev;
            bool isCurrent  = false;
            udev_list_entry_foreach(entry, devices) {
                const char *path = udev_list_entry_get_name(entry);
                entry_dev = udev_device_new_from_syspath(udev, path);
                std::string dev_name = udev_device_get_sysname(entry_dev);
                isCurrent |= dev_name == sysname;
                if (isCurrent && dev_name.find("event") != std::string::npos) {
                    if (extension->try_to_use_device(udev, entry_dev,lua)) {
                        std::string name = extension->getName();
                        name = name.substr(std::string("Nintendo Wii Remote ").size());
                        sol::table ext_table = extension->getLua_table();
                        ext_table["type"] = name;
                        lua_table["extension"] = extension->getLua_table();
                        lua_table["extension_name"] = name;
                    }
                }
                udev_device_unref(entry_dev);
            }

            udev_enumerate_unref(enumerate);
        }
        std::string sysname = udev_device_get_sysname(udev_device);

        return this->sysname == sysname || extension->getSysname() == sysname || ir->getSysname() == sysname || motion_plus->getSysname() == sysname || accelerometer->getSysname() == sysname;
    }
    sysname = udev_device_get_sysname(udev_device);
    const std::string devpath = "/dev/input/" + sysname;
    int fd = open(devpath.c_str(), O_RDONLY | O_NONBLOCK);
    struct libevdev *_dev = nullptr;
    int rc = libevdev_new_from_fd(fd, &_dev);
    if (rc < 0) {
        close(fd);
        return false;
    }
    const std::string found_name = libevdev_get_name(_dev);
    libevdev_free(_dev);
    if (found_name.find("Nintendo Wii") == std::string::npos) {
        return false;
    }
    struct udev_enumerate *enumerate = udev_enumerate_new(udev);
    //Find the parent wiimote hid device, and then start a search of its child devices
    udev_enumerate_add_match_parent(enumerate, udev_device_get_parent_with_subsystem_devtype(udev_device,"hid",nullptr));
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;
    struct udev_device *entry_dev;
    sol::table ext_table = lua.create_table_with("parent",lua_table);
    sol::table ir_table = lua.create_table_with("parent",lua_table,"type","Nintendo Wii Remote IR");
    sol::table accel_table = lua.create_table_with("parent",lua_table,"type","Nintendo Wii Remote Accelerometer");
    sol::table mp_table = lua.create_table_with("parent",lua_table,"type","Nintendo Wii Remote Motion Plus");
    if (!extension_name.empty()) {
        std::string name = "Nintendo Wii Remote " + extension_name;
        ext_table["type"] = name;
    }
    extension = Input::create(lua_name,ext_table);
    ir = Input::create(lua_name,ir_table);
    accelerometer = Input::create(lua_name,accel_table);
    motion_plus = Input::create(lua_name,mp_table);
    bool found = false;
    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        entry_dev = udev_device_new_from_syspath(udev, path);
        if (entry_dev == nullptr) continue;
        std::string dev_name = udev_device_get_sysname(entry_dev);
        if (dev_name.find("event") != std::string::npos) {
            if (Controller::try_to_use_device(udev,entry_dev,lua)) {
                found = true;
            }
            if (!motion_plus->isValid() && motion_plus->try_to_use_device(udev, entry_dev,lua)) {
                mp_table["type"] = "Motion Plus";
                lua_table["motion_plus"] = ir_table;
                udev_device_unref(entry_dev);
                continue;
            }
            if (!ir->isValid() && ir->try_to_use_device(udev, entry_dev,lua)) {
                ir_table["type"] = "IR";
                lua_table["ir"] = ir_table;
                udev_device_unref(entry_dev);
                continue;
            }
            if (!accelerometer->isValid() && accelerometer->try_to_use_device(udev, entry_dev,lua)) {
                accel_table["type"] = "Accelerometer";
                lua_table["accelerometer"] = accel_table;
                udev_device_unref(entry_dev);
                continue;
            }
            if (!extension->isValid() && extension->try_to_use_device(udev, entry_dev,lua)) {
                std::string name = extension->getName();
                name = name.substr(std::string("Nintendo Wii Remote ").size());
                ext_table["type"] = name;
                lua_table["extension"] = ext_table;
                lua_table["extension_name"] = name;
            }
        }
        udev_device_unref(entry_dev);
    }

    udev_enumerate_unref(enumerate);
    if ((!extension_name.empty() && !extension->isValid()) || !found) {
        try_disconnect(sysname, nullptr);
        extension->try_disconnect(extension->getSysname(), nullptr);
        ir->try_disconnect(ir->getSysname(), nullptr);
        accelerometer->try_disconnect(accelerometer->getSysname(), nullptr);
        motion_plus->try_disconnect(motion_plus->getSysname(), nullptr);
        return false;
    }
    return true;
}

void Wiimote::tick(sol::state &lua) {
    if (!isValid()) return;
    Controller::tick(lua);
    extension->tick(lua);
    ir->tick(lua);
    accelerometer->tick(lua);
    motion_plus->tick(lua);
}

Wiimote::~Wiimote() {
    delete(extension);
    delete(ir);
    delete(accelerometer);
    delete(motion_plus);
}

bool Wiimote::try_disconnect(const std::string &sysname,sol::state *lua) {
    if (!isValid())
        return false;
    if (extension->try_disconnect(sysname, lua)) {
        ir->try_disconnect(ir->getSysname(), lua);
        accelerometer->try_disconnect(accelerometer->getSysname(), lua);
        motion_plus->try_disconnect(motion_plus->getSysname(), lua);
        return Controller::try_disconnect(this->getSysname(), lua);
    }

    return Controller::try_disconnect(sysname, lua);
}
