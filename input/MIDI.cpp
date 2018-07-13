//
// Created by sanjay on 4/07/18.
//

#include "MIDIDirect.hpp"
#include "ControllerException.hpp"
#include "Wiimote.hpp"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include "buttons_ref.h"

MIDI::MIDI(const std::string &lua_name, const std::string &name, sol::table &lua_table): Controller(lua_name, name, lua_table) {}
void MIDI::parse_midi_command(char *buf, sol::state& lua)
{
    int operation, channel, param1, param2;

    operation = buf[0] & 0xF0;
    channel   = buf[0] & 0x0F;
    param1    = buf[1];
    param2    = buf[2];
    auto func = lua["midi_in"];
    if (func != nullptr) {
        func(lua_table, operation, channel, param1, param2);
    }
    switch (operation)
    {
        case 0x80:
            func = lua["midi_in_note_off"];
            if (func != nullptr) {
                func(lua_table, channel, param1, param2);
            }
            if (debug)
                printf("%s  0x%x Note off           %03u %03u %03u\n", lua_name.c_str(), operation, channel, param1, param2);
            break;

        case 0x90:
            func = lua["midi_in_note_on"];
            if (func != nullptr) {
                func(lua_table, channel, param1, param2);
            }
            if (debug)
                printf("%s  0x%x Note on            %03u %03u %03u\n", lua_name.c_str(),operation, channel, param1, param2);
            break;

        case 0xA0:
            func = lua["midi_in_pressure_change"];
            if (func != nullptr) {
                func(lua_table, channel, param1, param2);
            }
            if (debug)
                printf("%s  0x%x Pressure change    %03u %03u %03u\n", lua_name.c_str(),operation, channel, param1, param2);
            break;

        case 0xB0:
            func = lua["midi_in_controller_change"];
            if (func != nullptr) {
                func(lua_table, channel, param1, param2);
            }
            if (debug)
                printf("%s  0x%x Controller change  %03u %03u %03u\n", lua_name.c_str(),operation, channel, param1, param2);
            break;

        case 0xC0:
            func = lua["midi_in_program_change"];
            if (func != nullptr) {
                func(lua_table, channel, param1);
            }
            if (debug)
                printf("%s  0x%x Program change     %03u %03u\n", lua_name.c_str(),operation, channel, param1);
            break;

        case 0xD0:
            func = lua["midi_in_channel_change"];
            if (func != nullptr) {
                func(lua_table, channel, param1);
            }
            if (debug)
                printf("%s  0x%x Channel change     %03u %03u\n", lua_name.c_str(),operation, channel, param1);
            break;

        case 0xE0:
            param1 = (param1 & 0x7F) + ((param2 & 0x7F) << 7);
            func = lua["midi_in_pitch_bend"];
            if (func != nullptr) {
                func(lua_table, channel, param1);
            }
            if (debug)
                printf("%s  0x%x Pitch bend         %03u %05i\n", lua_name.c_str(),operation, channel, param1);
            break;

            /* Not implementing system commands (0xF0) */

        default:
            if (debug)
                printf("%s   0x%x Unknown MIDI cmd   %03u %03u %03u\n", lua_name.c_str(), operation, channel, param1, param2);
            break;
    }
}