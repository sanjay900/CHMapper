//
// Created by sanjay on 4/07/18.
//

#include "MIDIIn.hpp"
#include "src/DeviceException.hpp"
#include "Wiimote.hpp"
#include <utility>
#include <libudev.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>

CoreMIDIIn::CoreMIDIIn(const std::string &lua_name, sol::table &lua_table): Input(lua_name, "MIDI", lua_table) {
    this->debug = lua_table.get_or("debug",false);
}
int CoreMIDIIn::padding = 20;
std::map<unsigned char, std::string> CoreMIDIIn::func_map {
        {0x80, "note_off"},{0x90, "note_on"},{0xA0, "pressure_change"},
        {0xB0, "controller_change"},{0xC0, "program_change"},
        {0xD0, "channel_change"},{0xE0, "pitch_bend"},
        {0xF0, "sys_ex"}
};
//TODO: write functions that generate a message for each function above, and then support MIDI out for the hell of it
void CoreMIDIIn::parse_midi_command(unsigned char *buf, sol::state& lua)
{
    unsigned char operation, channel, param1, param2;

    operation = static_cast<unsigned char>(buf[0] & 0xF0u);
    channel   = static_cast<unsigned char>(buf[0] & 0x0Fu);
    param1    = buf[1];
    param2    = buf[2];
    auto func = lua["midi_in"];
    if (func.valid()) {
        func(lua_table, buf[0], buf[1], buf[2]);
    }
    sol::function note_function;
    std::string func_name = func_map[operation];
    if (func_name.empty()) {
        func_name = "unknown";
    }
    if (debug) {
        std::string debug_name = func_name;
        std::string temp_string;
        std::transform(debug_name.begin(), debug_name.begin()+1, debug_name.begin(), toupper);
        for (uint x = 0; x < debug_name.length(); x++){
            if (debug_name[x] == '_'){
                temp_string = debug_name.substr(x + 1, 1);
                std::transform(temp_string.begin(), temp_string.end(), temp_string.begin(), toupper);
                debug_name.erase(x, 2);
                debug_name.insert(x, " "+temp_string);
            }
        }
        if (operation >= 0xC0 && operation <= 0xD0) {
            printf("%s\t0x%x\t%-*s%03u %03u\n",lua_name.c_str(), operation, padding, debug_name.c_str(), channel, param1);
        } else if (operation == 0xE0) {
            param1 = static_cast<unsigned char>((param1 & 0x7Fu) + ((param2 & 0x7Fu) << 7u));
            printf("%s\t0x%x\t%-*s%03u %03u %05i\n",lua_name.c_str(), operation, padding, debug_name.c_str(), channel, param1, param2);
        } else {
            printf("%s\t0x%x\t%-*s%03u %03u %03u\n",lua_name.c_str(), operation, padding, debug_name.c_str(), channel, param1, param2);
        }
    }
    note_function = lua["midi_in_"+func_name];
    if (note_function.valid()) {
        note_function(lua_table, channel, param1, param2);
    }
}
