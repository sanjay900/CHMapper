#include "scanner.h"
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <iostream>
#include <algorithm>
#include "DeviceException.h"
#include <fcntl.h>
#include <unistd.h>
#include "input/WiiController.h"
#include "input/PS3.h"
#include "input/Raphnet.h"
#include <string.h>
Scanner::Scanner()
{
    struct udev *udev = udev_new();
    if (!udev)
    {
        fprintf(stderr, "udev_new() failed\n");
        return;
    }
    mon = udev_monitor_new_from_netlink(udev, "udev");

    udev_monitor_filter_add_match_subsystem_devtype(mon, "input", NULL);
    udev_monitor_enable_receiving(mon);

    fd = udev_monitor_get_fd(mon);
    tm.tv_sec = 0;
    tm.tv_usec = 0;
}
void Scanner::construct(udev_device *dev, std::list<Input *> *inputs)
{
    std::string dev_name = udev_device_get_sysname(dev);
    auto sysname = udev_device_get_syspath(dev);
    auto it = std::find_if(inputs->begin(), inputs->end(), [sysname](const Input *m) -> bool { return m->dev == sysname; });
    if (it != inputs->end())
    {
        return;
    }
    const std::string devpath = "/dev/input/" + dev_name;
    int fd = open(devpath.c_str(), O_RDONLY | O_NONBLOCK);
    struct libevdev *_dev = nullptr;
    int rc = libevdev_new_from_fd(fd, &_dev);
    if (rc < 0)
    {
        close(fd);
        return;
    }
    //Look for wii extensions, and associate them with their respective controllers.
    for (auto in : *inputs)
    {
        if (!in->has_children())
            continue;
        auto inBth = udev_device_get_syspath(udev_device_get_parent(udev_device_get_parent(udev_device_new_from_syspath(udev, in->dev.c_str()))));
        auto devBth = udev_device_get_syspath(udev_device_get_parent(udev_device_get_parent(dev)));
        if (inBth != nullptr && devBth != nullptr && std::string(inBth) == devBth)
        {
            Input *c = new Input("unknown", sysname, _dev, in->uidev);
            in->add_child(c);
            inputs->push_back(c);
            return;
        }
    }
    const std::string found_name = libevdev_get_name(_dev);
    auto vid = libevdev_get_id_vendor(_dev);
    auto pid = libevdev_get_id_product(_dev);
    Input *input = nullptr;
    if (found_name == "Nintendo Wii Remote")
    {
        input = new WiiController("Wii Remote", sysname, _dev);
    }
    if (vid == 0x289b && pid == 0x002b)
    {
        input = new Raphnet("Raphnet wusbmote", sysname, _dev);
    }
    if (vid == 0x12ba)
    {
        if (pid == 0x0210)
        {
            input = new PS3("PS3 Rock Band Drum Kit", sysname, _dev);
        }
        else if (pid == 0x0200)
        {
            input = new PS3("PS3 Rock Band Guitar", sysname, _dev);
        }
        else if (pid == 0x0100)
        {
            input = new PS3("PS3 Guitar Hero Guitar", sysname, _dev);
        }
        else if (pid == 0x0120)
        {
            input = new PS3("PS3 Guitar Hero Drum Kit", sysname, _dev);
        }
        else
        {
            input = new PS3("PS3 Controller", sysname, _dev);
        }
    }
    // construct and then call init function
    if (input != nullptr)
    {
        input->init();
        inputs->push_back(input);
    }
}
void Scanner::scan(std::list<Input *> *inputs)
{
    struct udev_device *dev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *device_list, *dev_list_entry;

    /* create udev object */
    udev = udev_new();
    if (!udev)
    {
        throw DeviceException("Cannot create udev context.");
    }
    /* create enumerate object */
    enumerate = udev_enumerate_new(udev);
    if (!enumerate)
    {
        throw DeviceException("Cannot create enumerate context.");
    }

    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);

    /* fillup device list */
    device_list = udev_enumerate_get_list_entry(enumerate);
    if (!device_list)
    {
        throw DeviceException("Failed to get device list.");
    }

    udev_list_entry_foreach(dev_list_entry, device_list)
    {
        const char *path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);
        if (dev == nullptr)
            continue;
        construct(dev, inputs);
        udev_device_unref(dev);
    }
    /* free enumerate */
    udev_enumerate_unref(enumerate);
}
void Scanner::findNew(std::list<Input *> *inputs)
{
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    if (select(fd + 1, &fds, NULL, NULL, &tm) > 0)
    {
        if (FD_ISSET(fd, &fds))
        {
            struct udev_device *dev = udev_monitor_receive_device(mon);
            const std::string sysname = udev_device_get_syspath(dev);
            const std::string action = udev_device_get_action(dev);
            if (action == "add")
            {
                construct(dev, inputs);
            }
            else if (action == "remove")
            {
                auto it = std::find_if(inputs->begin(), inputs->end(), [sysname](const Input *m) -> bool { return m->dev == sysname; });
                if (it != inputs->end())
                {
                    (*it)->disconnect();
                    inputs->erase(it);
                }
            }
        }
    }
}