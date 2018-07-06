#ifndef WEJOY_MIDI_H
#define WEJOY_MIDI_H


#include <cstdint>
#include "sol.hpp"

class MIDI {
    int fd;
    sol::table lua_table;
    std::string lua_name;

public:
    MIDI(const std::string &lua_name, sol::table &lua_table, sol::state&);
    ~MIDI();
};


#endif
