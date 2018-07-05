#ifndef WEJOY_VCONTROLLER_H
#define WEJOY_VCONTROLLER_H


#include <cstdint>
#include "sol.hpp"

class VJoy {
    struct libevdev_uinput *uidev;
    uint axes;
    uint buttons;
    uint64_t buttonFlags = 0;
    std::vector<uint16_t> axesData;
    sol::table lua_table;
    std::string lua_name;
    void send_button_event(uint, bool);

    void send_axis_event(uint, uint16_t);
    bool get_button_status(int);
    uint16_t get_axis_status(int t) { return axesData[t]; };
public:
    VJoy(const std::string &lua_name, sol::table &lua_table);
    ~VJoy();
};


#endif
