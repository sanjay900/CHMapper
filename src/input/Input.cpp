#include "Input.h"
#include <iostream>
#include <string.h>
#include <cmath>
#include "../DeviceException.h"

static int buttons[] = {BTN_SOUTH, BTN_EAST, BTN_NORTH, BTN_WEST, BTN_TL, BTN_TR, BTN_SELECT, BTN_START, BTN_MODE, BTN_THUMBL, BTN_THUMBR};
// static int axis[] = {ABS_X, ABS_Y, ABS_Z, ABS_RX, ABS_RY, ABS_RZ, ABS_HAT0X, ABS_HAT0Y};
std::unordered_map<int, input_absinfo> axis = {
    {ABS_X, {0,-32768, 32767}}, {ABS_Y, {0,-32768, 32767}}, {ABS_Z, {0,0, 255}}, {ABS_RX, {0,-32768, 32767}}, {ABS_RY, {0,-32768, 32767}}, {ABS_RZ, {0,0, 255}}, {ABS_HAT0X, {0,-1, 1}}, {ABS_HAT0Y, {0,-1, 1}}};

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
    struct input_event ev
    {
    };
    rc = libevdev_next_event(_dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
    if (rc == 0)
    {
        if (ev.type == EV_KEY)
        {
            libevdev_uinput_write_event(uidev, EV_KEY, buttonMap[ev.code], ev.value);
            libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
            //BTN_DPAD_UP, BTN_DPAD_DOWN, BTN_DPAD_LEFT, BTN_DPAD_RIGHT
            if (buttonMap[ev.code] == BTN_DPAD_UP)
            {
                libevdev_uinput_write_event(uidev, EV_ABS, ABS_HAT0Y, ev.value ? -1 : 0);
                libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
            }
            if (buttonMap[ev.code] == BTN_DPAD_DOWN)
            {
                libevdev_uinput_write_event(uidev, EV_ABS, ABS_HAT0Y, ev.value ? 1 : 0);
                libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
            }
            if (buttonMap[ev.code] == BTN_DPAD_LEFT)
            {
                libevdev_uinput_write_event(uidev, EV_ABS, ABS_HAT0X, ev.value ? -1 : 0);
                libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
            }
            if (buttonMap[ev.code] == BTN_DPAD_RIGHT)
            {
                libevdev_uinput_write_event(uidev, EV_ABS, ABS_HAT0X, ev.value ? 1 : 0);
                libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
            }
        }
        auto a = axisMap.find(ev.code);
        if (ev.type == EV_ABS && a != axisMap.end())
        {
            auto a2 = axis[a->second];
            //How fast is this, should we be caching this?
            int val = scale(ev.value, libevdev_get_abs_minimum(_dev, ev.code), libevdev_get_abs_maximum(_dev, ev.code), a2.minimum, a2.maximum);
            auto func = axisMapScaled[a->second];
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
    if (err != 0)
    {
        throw DeviceException(
            strerror(-err) + std::string(": Failed creating virtual device ") + dev_name + ".");
    }
}