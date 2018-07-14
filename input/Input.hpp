//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_Input_H
#define WEJOY_Input_H


#include <map>
#include "sol.hpp"

class Input {
protected:
    sol::table lua_table;
    std::string lua_name;
    std::string name;
    std::string sysname;
public:
    const sol::table &getLua_table() const;

    const std::string &getSysname() const;

    Input(const std::string&, const std::string&, sol::table&);
    virtual bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) = 0;
    virtual bool try_disconnect(const std::string &sysname,sol::state *lua) = 0;
    const std::string &getLua_name() const;

    const std::string &getName() const;

    virtual bool isValid() const = 0;

    virtual void tick(sol::state &lua) = 0;

    static Input* create(std::string &name, sol::table &lua_table);

    friend bool operator<(const Input &lhs, const Input &rhs);

    friend bool operator>(const Input &lhs, const Input &rhs);

    friend bool operator<=(const Input &lhs, const Input &rhs);

    friend bool operator>=(const Input &lhs, const Input &rhs);
};


#endif
