#ifndef WEJOY_VKEY_H
#define WEJOY_VKEY_H


#include <cstdint>
#include "sol.hpp"

class VKeyboard {
    struct libevdev_uinput *uidev;
    std::vector<bool> keyFlags;
    sol::table lua_table;
    std::string lua_name;
    void send_key(uint, bool);
    bool get_key(int i) {
        return keyFlags[i];
    }

public:
    VKeyboard(const std::string &lua_name, sol::table &lua_table, sol::state&);
    ~VKeyboard();
};


#endif
