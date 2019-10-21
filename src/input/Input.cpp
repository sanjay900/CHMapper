#include "Input.h"
#include <iostream>
#include <string.h>
#include <cmath>
#include "../DeviceException.h"

static int buttons[] = {BTN_SOUTH, BTN_EAST, BTN_NORTH, BTN_WEST, BTN_TL, BTN_TR, BTN_SELECT, BTN_START, BTN_MODE, BTN_THUMBL, BTN_THUMBR};
std::unordered_map<int, input_absinfo> axis = {
    {ABS_X, {0, -32768, 32767}}, {ABS_Y, {0, -32768, 32767}}, {ABS_Z, {0, 0, 255}}, {ABS_RX, {0, -32768, 32767}}, {ABS_RY, {0, -32768, 32767}}, {ABS_RZ, {0, 0, 255}}, {ABS_HAT0X, {0, -1, 1}}, {ABS_HAT0Y, {0, -1, 1}}};

Input::Input(std::string dev, struct libevdev *_dev) : dev(dev), _dev(_dev)
{
    libevdev_grab(_dev, LIBEVDEV_GRAB);
}
Input::Input(std::string dev, struct libevdev *_dev, struct libevdev_uinput *uidev) : dev(dev), _dev(_dev), uidev(uidev)
{
    libevdev_grab(_dev, LIBEVDEV_GRAB);
}
void Input::disconnect()
{
    std::cout << "Device has disconnected" << std::endl;
    libevdev_uinput_destroy(uidev);
}
void Input::add_child(Input *input)
{
}
int scale(int x, int in_min, int in_max, int out_min, int out_max)
{
    return static_cast<int>(std::floor((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min));
}
void Input::tick()
{
    int rc;
    struct input_event ev{};
    rc = libevdev_next_event(_dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
    if (rc == 0)
    {
        auto a = buttonMap.find(ev.code);
        if (ev.type == EV_KEY)
        {
            //Allow mapping buttons to axis
            if (a != buttonMap.end())
            {
                libevdev_uinput_write_event(uidev, EV_KEY, buttonMap[ev.code], ev.value);
                libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
            }
            else
            {
                a = axisMap.find(ev.code);
                if (a != axisMap.end())
                {
                    int signum = std::signbit(a->second);
                    auto ax = std::abs(a->second);
                    auto a2 = axis[ax];
                    auto val = ev.value ? (signum ? a2.maximum : a2.minimum) : 0;
                    libevdev_uinput_write_event(uidev, EV_ABS, ax, val);
                    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
                }
            }
        }
        a = axisMap.find(ev.code);
        if (ev.type == EV_ABS && a != axisMap.end())
        {
            auto a2 = axis[a->second];
            //How fast is this, should we be caching this?
            int val = scale(ev.value, libevdev_get_abs_minimum(_dev, ev.code), libevdev_get_abs_maximum(_dev, ev.code), a2.minimum, a2.maximum + 1);
            auto func = axisMapScaled[ev.code];
            if (func != nullptr)
            {
                val = func(val);
            }
            libevdev_uinput_write_event(uidev, EV_ABS, a->second, val);
            libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
        }
    }
}

void Input::init()
{
    std::string dev_name = "CHMapper Virtual Device";
    struct libevdev *evdev;
    evdev = libevdev_new();
    libevdev_set_name(evdev, dev_name.c_str());
    libevdev_set_id_bustype(evdev, 3);
    libevdev_set_id_vendor(evdev, 0x045e);
    libevdev_set_id_product(evdev, 0x02a1);
    libevdev_set_id_version(evdev, 0x630);
    for (int button : buttons)
    {
        libevdev_enable_event_code(evdev, EV_KEY, button, nullptr);
    }
    for (auto a : axis)
    {
        libevdev_enable_event_code(evdev, EV_ABS, a.first, &a.second);
    }
    int err = libevdev_uinput_create_from_device(evdev,
                                                 LIBEVDEV_UINPUT_OPEN_MANAGED,
                                                 &uidev);
    if (err == -ENOENT) {
        std::cout << "Unable to communicate with uinput module, attempting to enable." << std::endl;
        system("sudo modprobe uinput");
        int err = libevdev_uinput_create_from_device(evdev,
                                                 LIBEVDEV_UINPUT_OPEN_MANAGED,
                                                 &uidev);
        if (err == -ENOENT) {
            std::cout << "Unable to automatically load uinput, please install it and then run sudo modprobe uinput" << std::endl;
        }
    }
    if (err != 0)
    {
        throw DeviceException(
            strerror(-err) + std::string(": Failed creating virtual device ") + dev_name + ".");
    }
}