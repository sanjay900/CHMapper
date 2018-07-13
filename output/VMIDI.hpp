#ifndef WEJOY_VMIDI_H
#define WEJOY_VMIDI_H


#include <cstdint>
#include "sol.hpp"

class VMIDI {
    int fd;
    sol::table lua_table;
    std::string lua_name;

public:
    VMIDI(const std::string &lua_name, sol::table &lua_table, sol::state&);
    ~VMIDI();
};


#endif
