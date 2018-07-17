#ifndef WEJOY_VKEY_H
#define WEJOY_VKEY_H


#include <cstdint>
#include "src/sol.hpp"
#include "Output.hpp"

class VKeyboard : public Output{
    struct libevdev_uinput *uidev;
    std::vector<bool> keyFlags;
    sol::table lua_table;
    std::string lua_name;
    void send_key(uint, bool);
    bool get_key(int i) {
        return keyFlags[i];
    }

    bool try_to_use_device(struct udev *udev, struct udev_device *device, sol::state &lua) override;

    bool try_disconnect(const std::string &sysname, sol::state *lua) override;

public:
    VKeyboard(const std::string &lua_name, sol::table &lua_table, sol::state&);
    ~VKeyboard();
};


#endif