//
// Created by sanjay on 4/07/18.
//

#include <libudev.h>
#include "Wiimote.h"
#include "ControllerException.h"

Wiimote::Wiimote(const std::string &name, const std::string &dev_name, sol::table &dev): Controller(name, dev_name, dev) {
    sol::optional<std::string> ext = lua_table["extension_type"];
    if (ext != sol::nullopt) {
        extension_name = ext.value();
    }
}


bool Wiimote::try_to_use_device(struct udev * udev, struct udev_device * udev_device, sol::state &lua) {
    if (!Controller::try_to_use_device(udev,udev_device,lua)) {
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
    sol::table new_table = lua.create_table_with("parent",lua_table);
    if (!extension_name.empty()) {
        std::string name = "Nintendo Wii Remote " + extension_name;
        new_table["type"] = name;
    }
    extension = Controller::create(name,new_table);
    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        entry_dev = udev_device_new_from_syspath(udev, path);
        std::string dev_name = udev_device_get_sysname(entry_dev);
        if (dev_name.find("event") != std::string::npos) {
            if (extension->try_to_use_device(udev, entry_dev,lua)) {
                std::string name = extension->getName();
                name = name.substr(std::string("Nintendo Wii Remote ").size());
                extension->lua_table["name"] = name;
                lua_table["extension"] = extension->lua_table;
                lua_table["extension_name"] = name;
                udev_device_unref(entry_dev);
                break;
            }
        }
        udev_device_unref(entry_dev);
    }

    udev_enumerate_unref(enumerate);
    return true;
}