//
// Created by sanjay on 4/07/18.
//

#ifndef WEJOY_CONTROLLER_H
#define WEJOY_CONTROLLER_H


#include <map>
#include "sol.hpp"

class Controller {
    sol::table lua_table;
    std::string lua_name;
    std::string name;
    bool valid = false;
    struct libevdev *dev = nullptr;
    std::vector<int> axisBindings;
    std::map<int,int> typeBindings;
protected:
    Controller(const std::string&, const std::string&, sol::table&);
public:
    static Controller* create(std::string&,sol::table&);
    bool try_to_use_device(struct udev*, struct udev_device*);
    const std::string &getLua_name() const;

    const std::string &getName() const;

    bool isValid() const;
};


#endif
