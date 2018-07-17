#ifndef WEJOY_VMIDI_H
#define WEJOY_VMIDI_H


#include <cstdint>
#include "src/sol.hpp"
#include "Output.hpp"

class VMIDI : public Output{
    int fd;
    sol::table lua_table;
    std::string lua_name;

    bool try_to_use_device(struct udev *udev, struct udev_device *device, sol::state &lua) override;

    bool try_disconnect(const std::string &sysname, sol::state *lua) override;

public:
    VMIDI(const std::string &lua_name, sol::table &lua_table, sol::state&);
    ~VMIDI();
};


#endif
