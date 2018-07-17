#ifndef WEJOY_VCONTROLLER_H
#define WEJOY_VCONTROLLER_H


#include <cstdint>
#include "src/sol.hpp"
#include "Output.hpp"

class VJoy : public Output {
    struct libevdev_uinput *uidev;
    uint axes;
    uint buttons;
    uint64_t buttonFlags = 0;
    std::vector<int> axesData;
    sol::table lua_table;

    bool try_to_use_device(struct udev *udev, struct udev_device *device, sol::state &lua) override;

    bool try_disconnect(const std::string &sysname, sol::state *lua) override;

    std::string lua_name;
    void send_button_event(uint, bool);

    void send_axis_event(uint, int);
    bool get_button_status(int);
    int get_axis_status(int t) { return axesData[t]; };
public:
    VJoy(const std::string &lua_name, sol::table &lua_table);
    ~VJoy();
};


#endif
