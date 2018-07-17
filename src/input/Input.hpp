//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_Input_H
#define WEJOY_Input_H


#include <map>
#include <src/Device.hpp>
#include "src/sol.hpp"

class Input : public virtual Device {
protected:
    sol::table lua_table;
    std::string name;
    std::string sysname;
public:
    const sol::table &getLua_table() const;

    const std::string &getSysname() const;

    Input(const std::string&, const std::string&, sol::table&);
    const std::string &getLua_name() const;

    const std::string &getName() const;

    virtual bool isValid() const = 0;

    virtual void tick(sol::state &lua) = 0;

    static Input* create(std::string &name, sol::table &lua_table);
};


#endif
