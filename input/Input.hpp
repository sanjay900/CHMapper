//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_Input_H
#define WEJOY_Input_H


#include <map>
#include "sol.hpp"

class Input {
    friend class Wiimote;
    friend class InputFactory;
    friend class MIDIDirect;
    friend class MIDI;
    friend class Serial;
    friend class MIDISerial;
    friend class Controller;
    sol::table lua_table;
    std::string lua_name;
    std::string name;
    std::string sysname;
    int fd = -1;
protected:
    Input(const std::string&, const std::string&, sol::table&);
public:
    virtual bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) = 0;
    virtual bool try_disconnect(const std::string &sysname,sol::state *lua) = 0;
    const std::string &getLua_name() const;

    const std::string &getName() const;

    virtual bool isValid() const;

    virtual void tick(sol::state &lua) = 0;

    friend bool operator<(const Input &lhs, const Input &rhs);

    friend bool operator>(const Input &lhs, const Input &rhs);

    friend bool operator<=(const Input &lhs, const Input &rhs);

    friend bool operator>=(const Input &lhs, const Input &rhs);
};


#endif
