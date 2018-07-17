//
// Created by sanjay on 18/07/18.
//

#ifndef SANPIE_DEVICE_H
#define SANPIE_DEVICE_H


#include "sol.hpp"

class Device {
protected:
    std::string lua_name;
public:
    virtual bool try_to_use_device(struct udev*, struct udev_device*, sol::state &lua) = 0;
    virtual bool try_disconnect(const std::string &sysname,sol::state *lua) = 0;

    friend bool operator<(const Device &lhs, const Device &rhs);

    friend bool operator>(const Device &lhs, const Device &rhs);

    friend bool operator<=(const Device &lhs, const Device &rhs);

    friend bool operator>=(const Device &lhs, const Device &rhs);
};


#endif //SANPIE_DEVICE_H
